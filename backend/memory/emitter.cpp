#include "compiler.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace memory {
namespace {
struct Plan {
    Loop loop;
    Candidate chosen;
    int fusedEnd = -1;
    long threshold = 0;
};
class Emitter {
    const Function& f;
    std::ostream& out;
    std::map<std::string,long> slots;
    long frame = 0, cpuSlot = 0, laneBase = 0;
    int serial = 0;
    std::string ret;
    bool vectors = false;
    std::string slot(const std::string& s) const {
        if (!slots.count(s)) throw std::runtime_error("unknown symbol (globals unsupported): " + s);
        return std::to_string(slots.at(s))+"(%rbp)";
    }
    const Symbol& sym(const std::string& s) const {
        if (!f.symbols.count(s)) throw std::runtime_error("unknown symbol: " + s);
        return f.symbols.at(s);
    }
    bool pointer(const std::string& s) const { return !number(s) && sym(s).pointer(); }
    void ins(const std::string& s) { out << "    " << s << '\n'; }
    std::string label(const std::string& name) const { return ".L"+f.name+"_"+name; }
    std::string fresh(const std::string& purpose) { return label(purpose+std::to_string(serial++)); }
    void load32(const std::string& s, const std::string& r) {
        if (number(s)) {
            long v = std::stol(s);
            if (v < -2147483648L || v > 2147483647L) throw std::runtime_error("integer literal outside int32");
            ins("movl $"+s+", "+r);
        } else {
            if (sym(s).type != "i32") throw std::runtime_error("expected int32 operand: "+s);
            ins("movl "+slot(s)+", "+r);
        }
    }
    void load64(const std::string& s, const std::string& r) {
        if (number(s)) ins("movq $"+s+", "+r);
        else if (sym(s).pointer()) ins("movq "+slot(s)+", "+r);
        else if (sym(s).type == "i32") ins("movslq "+slot(s)+", "+r);
        else throw std::runtime_error("expected pointer/integer operand");
    }
    void base(const std::string& s, const std::string& r) {
        if (sym(s).array()) ins("leaq "+slot(s)+", "+r);
        else if (sym(s).pointer()) ins("movq "+slot(s)+", "+r);
        else throw std::runtime_error("expected array base");
    }
    std::string lane(int id) const { return std::to_string(laneBase-8*id)+"(%rbp)"; }
    void body(const std::vector<Node>& nodes, bool vector) {
        std::vector<int> uses(nodes.size()), regs(nodes.size(),-1);
        std::set<int> free;
        for (int r = 0; r < 16; ++r) free.insert(r);
        for (const Node& n : nodes) { if (n.a >= 0) ++uses[n.a]; if (n.b >= 0) ++uses[n.b]; }
        auto vr = [&](int id) { return "%ymm"+std::to_string(regs.at(id)); };
        for (int i = 0; i < int(nodes.size()); ++i) {
            const Node& n = nodes[i];
            if (vector && n.op != "store") {
                if (free.empty()) throw std::runtime_error("internal error: vector register pressure");
                regs[i] = *free.begin(); free.erase(free.begin());
            }
            if (n.op == "load" || n.op == "store") {
                base(n.token,"%r10");
                if (vector) {
                    if (n.op == "load") ins("vmovdqu (%r10,%r8,4), "+vr(i));
                    else ins("vmovdqu "+vr(n.a)+", (%r10,%r8,4)");
                } else {
                    if (n.op == "load") { ins("movl (%r10,%r8,4), %eax"); ins("movl %eax, "+lane(i)); }
                    else { ins("movl "+lane(n.a)+", %eax"); ins("movl %eax, (%r10,%r8,4)"); }
                }
            } else if (n.op == "broadcast") {
                load32(n.token,"%eax");
                if (vector) { ins("vmovd %eax, %xmm"+std::to_string(regs[i])); ins("vpbroadcastd %xmm"+std::to_string(regs[i])+", "+vr(i)); }
                else ins("movl %eax, "+lane(i));
            } else if (vector) {
                std::string op = n.op == "+" ? "vpaddd" : n.op == "-" ? "vpsubd" : "vpmulld";
                ins(op+" "+vr(n.b)+", "+vr(n.a)+", "+vr(i));
            } else {
                ins("movl "+lane(n.a)+", %eax");
                ins(std::string(n.op == "+" ? "addl" : n.op == "-" ? "subl" : "imull")+" "+lane(n.b)+", %eax");
                ins("movl %eax, "+lane(i));
            }
            if (vector) {
                if (n.a >= 0 && --uses[n.a] == 0) free.insert(regs[n.a]);
                if (n.b >= 0 && --uses[n.b] == 0) free.insert(regs[n.b]);
                if (n.op != "store" && uses[i] == 0) free.insert(regs[i]);
            }
        }
    }
    void optimized(const Plan& p) {
        const Loop& l = p.loop;
        std::string fallback = label(f.code[l.header].result);
        std::string vectorLoop = fresh("vector"), tail = fresh("tail"), done = fresh("done");
        if (p.threshold > 0) {
            // Analysis proves immediate constant initialization on the sole entry
            // to this prefix. bound < start+threshold tests the actual trip count
            // without subtraction or duplicate bound loads; scalar back edges skip it.
            load64(l.bound,"%r9");
            ins("cmpq $"+std::to_string(l.start+p.threshold)+", %r9"); ins("jl "+fallback);
        }
        ins("cmpl $0, "+std::to_string(cpuSlot)+"(%rbp)"); ins("je "+fallback);
        load64(l.iv,"%r8");
        if (p.threshold == 0) load64(l.bound,"%r9");
        // Match signed int32 byte offsets from the source TAC, including dynamic bounds.
        ins("cmpq $536870911, %r9"); ins("jg "+fallback);
        ins("cmpq %r8, %r9"); ins("jle "+fallback);
        // Guard local extents too. Invalid source inputs retain the original scalar path.
        std::set<std::string> all = l.reads; all.insert(l.writes.begin(),l.writes.end());
        std::vector<std::string> pointers;
        for (const auto& b : all) {
            if (sym(b).array()) { ins("cmpq $"+std::to_string(sym(b).count)+", %r9"); ins("jg "+fallback); }
            else pointers.push_back(b);
        }
        if (l.guards) {
            // Full [base+start*4, base+bound*4) intervals, with overflow checks.
            for (size_t a = 0; a < pointers.size(); ++a) for (size_t b = a+1; b < pointers.size(); ++b) {
                std::string disjoint = fresh("disjoint");
                base(pointers[a],"%rax"); base(pointers[b],"%rcx");
                ins("movq %r9, %rdx"); ins("shlq $2, %rdx"); ins("movq %rdx, %rsi");
                ins("addq %rax, %rdx"); ins("jc "+fallback);
                ins("addq %rcx, %rsi"); ins("jc "+fallback);
                ins("addq $"+std::to_string(4*l.start)+", %rax"); ins("jc "+fallback);
                ins("addq $"+std::to_string(4*l.start)+", %rcx"); ins("jc "+fallback);
                ins("cmpq %rsi, %rax"); ins("jae "+disjoint);
                ins("cmpq %rdx, %rcx"); ins("jae "+disjoint);
                ins("jmp "+fallback); out << disjoint << ":\n";
            }
        }
        ins("movq %r9, %r11"); ins("subq %r8, %r11"); ins("andq $-8, %r11"); ins("addq %r8, %r11");
        ins("cmpq %r11, %r8"); ins("jge "+tail);
        out << vectorLoop << ":\n";
        body(p.chosen.nodes,true);
        ins("addq $8, %r8"); ins("cmpq %r11, %r8"); ins("jl "+vectorLoop);
        out << tail << ":\n";
        ins("vzeroupper");
        if (p.fusedEnd < 0) {
            ins("movl %r8d, "+slot(l.iv)); ins("jmp "+fallback);
        } else {
            std::string remainder = fresh("remainder");
            ins("cmpq %r9, %r8"); ins("jge "+done);
            out << remainder << ":\n";
            body(p.chosen.nodes,false);
            ins("incq %r8"); ins("cmpq %r9, %r8"); ins("jl "+remainder);
            out << done << ":\n";
            ins("movl %r8d, "+slot(l.iv)); ins("jmp "+label(f.code[p.fusedEnd].result));
        }
    }
    void scalar(const IRQuad& q, std::vector<std::string>& args) {
        if (q.op == "label") { out << label(q.result) << ":\n"; return; }
        if (q.op == "=") {
            if (pointer(q.result)) { load64(q.arg1,"%rax"); ins("movq %rax, "+slot(q.result)); }
            else { load32(q.arg1,"%eax"); ins("movl %eax, "+slot(q.result)); }
        } else if (q.op == "=&") {
            if (!pointer(q.result)) throw std::runtime_error("address result must be pointer");
            ins("leaq "+slot(q.arg1)+", %rax"); ins("movq %rax, "+slot(q.result));
        } else if (q.op == "=*") {
            if (!pointer(q.arg1) || sym(q.result).type != "i32") throw std::runtime_error("only int32 dereferences supported");
            load64(q.arg1,"%rax"); ins("movl (%rax), %ecx"); ins("movl %ecx, "+slot(q.result));
        } else if (q.op == "*=") {
            if (!pointer(q.arg1)) throw std::runtime_error("store address must be pointer");
            load64(q.arg1,"%rax"); load32(q.arg2,"%ecx"); ins("movl %ecx, (%rax)");
        } else if (q.op == "+" || q.op == "-" || q.op == "*" || q.op == "/" || q.op == "%" ||
                   q.op == "&" || q.op == "|" || q.op == "^" || q.op == "<<" || q.op == ">>" || q.op == "~") {
            bool wide = pointer(q.result) || pointer(q.arg1) || (!q.arg2.empty() && pointer(q.arg2));
            if (wide) {
                if ((q.op != "+" && q.op != "-") || q.arg2.empty()) throw std::runtime_error("unsupported pointer operation");
                load64(q.arg1,"%rax"); load64(q.arg2,"%rcx"); ins((q.op == "+" ? "addq" : "subq")+std::string(" %rcx, %rax"));
                ins(std::string(pointer(q.result) ? "movq %rax, " : "movl %eax, ")+slot(q.result));
            } else {
                load32(q.arg1,"%eax");
                if (q.arg2.empty()) {
                    if (q.op == "-") ins("negl %eax");
                    else if (q.op == "~") ins("notl %eax");
                    else if (q.op != "+") throw std::runtime_error("unsupported unary opcode");
                } else {
                    load32(q.arg2,"%ecx");
                    if (q.op == "/" || q.op == "%") { ins("cltd"); ins("idivl %ecx"); if (q.op == "%") ins("movl %edx, %eax"); }
                    else {
                        const std::map<std::string,std::string> ops = {{"+","addl"},{"-","subl"},{"*","imull"},{"&","andl"},{"|","orl"},{"^","xorl"},{"<<","sall"},{">>","sarl"}};
                        if (!ops.count(q.op)) throw std::runtime_error("bad arithmetic opcode");
                        ins(ops.at(q.op)+std::string((q.op == "<<" || q.op == ">>") ? " %cl, %eax" : " %ecx, %eax"));
                    }
                }
                ins("movl %eax, "+slot(q.result));
            }
        } else if (q.op == "goto") ins("jmp "+label(q.result));
        else if (q.op.compare(0,2,"if") == 0) {
            if (q.op == "if") { load32(q.arg1,"%eax"); ins("testl %eax, %eax"); ins("jne "+label(q.result)); }
            else {
                bool wide = pointer(q.arg1) || pointer(q.arg2);
                if (wide) { load64(q.arg1,"%rax"); load64(q.arg2,"%rcx"); ins("cmpq %rcx, %rax"); }
                else { load32(q.arg1,"%eax"); load32(q.arg2,"%ecx"); ins("cmpl %ecx, %eax"); }
                std::map<std::string,std::string> jumps = {{"if<",wide?"jb":"jl"},{"if>",wide?"ja":"jg"},{"if<=",wide?"jbe":"jle"},{"if>=",wide?"jae":"jge"},{"if==","je"},{"if!=","jne"}};
                if (!jumps.count(q.op)) throw std::runtime_error("bad comparison");
                ins(jumps.at(q.op)+" "+label(q.result));
            }
        } else if (q.op == "param") args.push_back(q.arg1);
        else if (q.op == "call") {
            if (args.size() > 6 || !number(q.arg2) || std::stol(q.arg2) != long(args.size())) throw std::runtime_error("invalid call arguments");
            static const char* r64[] = {"%rdi","%rsi","%rdx","%rcx","%r8","%r9"};
            static const char* r32[] = {"%edi","%esi","%edx","%ecx","%r8d","%r9d"};
            for (size_t i = 0; i < args.size(); ++i) {
                if (pointer(args[i])) load64(args[i],r64[i]); else load32(args[i],r32[i]);
            }
            ins("xorl %eax, %eax"); ins("call "+q.arg1); args.clear();
            if (!q.result.empty()) { if (sym(q.result).type != "i32") throw std::runtime_error("noninteger call result"); ins("movl %eax, "+slot(q.result)); }
        } else if (q.op == "return") {
            if (!q.arg1.empty()) load32(q.arg1,"%eax"); else ins("xorl %eax, %eax");
            ins("jmp "+ret);
        } else throw std::runtime_error("typed scalar backend does not implement " + q.op);
    }
public:
    Emitter(const Function& f_, std::ostream& out_) : f(f_), out(out_), ret(label("return")) {}
    void emit(const std::map<int,Plan>& plans) {
        size_t lanes = 0;
        for (const auto& p : plans) { vectors = true; lanes = std::max(lanes,p.second.chosen.nodes.size()); }
        for (const auto& s : f.symbols) {
            frame += s.second.array() ? ((s.second.count*4+7)/8)*8 : 8;
            slots[s.first] = -frame;
        }
        frame += 8; cpuSlot = -frame;
        frame += 8; laneBase = -frame; frame += lanes*8;
        frame = (frame+15)/16*16;
        if (frame > 8*1024*1024) throw std::runtime_error("stack frame exceeds supported 8 MiB");
        out << ".text\n.globl " << f.name << "\n.type " << f.name << ", @function\n" << f.name << ":\n";
        ins("pushq %rbp"); ins("movq %rsp, %rbp"); ins("subq $"+std::to_string(frame)+", %rsp");
        static const char* r64[] = {"%rdi","%rsi","%rdx","%rcx","%r8","%r9"};
        static const char* r32[] = {"%edi","%esi","%edx","%ecx","%r8d","%r9d"};
        for (const auto& s : f.symbols) if (s.second.param >= 0)
            ins(std::string(s.second.pointer() ? "movq " : "movl ")+(s.second.pointer() ? r64[s.second.param] : r32[s.second.param])+", "+slot(s.first));
        if (vectors) { ins("call .Lmemory_cpu_check"); ins("movl %eax, "+std::to_string(cpuSlot)+"(%rbp)"); }
        std::vector<std::string> args;
        for (int i = 0; i < int(f.code.size()); ++i) {
            if (plans.count(i)) optimized(plans.at(i));
            if (!args.empty() && f.code[i].op != "param" && f.code[i].op != "call") throw std::runtime_error("nonadjacent param/call TAC unsupported");
            scalar(f.code[i],args);
        }
        if (!args.empty()) throw std::runtime_error("unconsumed call parameters");
        ins("xorl %eax, %eax"); out << ret << ":\n";
        ins("leave"); ins("ret"); out << ".size " << f.name << ", .-" << f.name << '\n';
    }
};
void cpuHelper(std::ostream& out) {
    out << R"ASM(
.data
.p2align 2
.Lmemory_cpu_state: .long -1
.text
.Lmemory_cpu_check:
    movl .Lmemory_cpu_state(%rip), %eax
    cmpl $-1, %eax
    jne .Lmemory_cpu_return
    pushq %rbx
    movl $1, %eax
    cpuid
    andl $0x18000000, %ecx
    cmpl $0x18000000, %ecx
    jne .Lmemory_cpu_no
    xorl %ecx, %ecx
    xgetbv
    andl $6, %eax
    cmpl $6, %eax
    jne .Lmemory_cpu_no
    movl $7, %eax
    xorl %ecx, %ecx
    cpuid
    btl $5, %ebx
    setc %al
    movzbl %al, %eax
    jmp .Lmemory_cpu_save
.Lmemory_cpu_no:
    xorl %eax, %eax
.Lmemory_cpu_save:
    popq %rbx
    movl %eax, .Lmemory_cpu_state(%rip)
.Lmemory_cpu_return:
    ret
)ASM";
}
Candidate candidate(const std::string& name, const std::vector<Node>& nodes, const Loop& l, const Function& f, const Weights& w) {
    Candidate c; c.name = name; c.nodes = nodes; c.stats = statistics(nodes); c.cost = estimate(l,c,f,w); return c;
}
std::vector<Candidate> alternatives(const Loop& l, const Function& f, const Weights& w, std::ostream* trace = nullptr) {
    std::vector<Candidate> choices = {candidate("scalar",l.nodes,l,f,w),candidate("simd",l.nodes,l,f,w)};
    auto reused = reuseLoads(l.nodes);
    Candidate reuse = candidate("reuse",reused,l,f,w);
    reuse.reusedLoads = choices[1].stats.loads - reuse.stats.loads;
    if (reuse.stats.peak <= 16) choices.push_back(reuse);
    else if (trace) *trace << "  reuse_rejected=vector register pressure exceeds 16\n";
    auto forwarded = forwardStoredValues(l,reused);
    if (!forwarded.rejection.empty()) {
        if (trace) *trace << "  forward_rejected=" << forwarded.rejection << '\n';
    } else {
        Candidate forward = candidate("forward",forwarded.nodes,l,f,w);
        forward.reusedLoads = reuse.reusedLoads;
        forward.forwardedLoads = forwarded.eliminated;
        if (forward.stats.peak <= 16) choices.push_back(forward);
        else if (trace) *trace << "  forward_rejected=vector register pressure " << forward.stats.peak << " exceeds 16; retain previous legal candidate\n";
    }
    return choices;
}
void show(std::ostream& out, const Candidate& c, bool dumpIR = false) {
    out << "  candidate=" << c.name << " cost=" << std::fixed << std::setprecision(3) << c.cost
        << " loads=" << c.stats.loads << " stores=" << c.stats.stores << " alu=" << c.stats.alu
        << " broadcasts=" << c.stats.broadcasts << " peak_registers=" << c.stats.peak
        << " reused_loads=" << c.reusedLoads << " forwarded_loads=" << c.forwardedLoads << '\n';
    if (dumpIR) for (int i = 0; i < int(c.nodes.size()); ++i) {
        const Node& n = c.nodes[i];
        out << "    ir=" << c.name << " id=" << i << " op=" << n.op << " token=" << n.token
            << " a=" << n.a << " b=" << n.b << '\n';
    }
}
}
void compile(const Program& p, const Options& options, std::ostream& assembly, std::ostream& trace) {
    bool anyVectors = false;
    trace << "cost units=" << options.weights.units << "; unknown bounds assume trip=" << options.weights.assumedTrip
          << "; alias_policy=" << options.alias << '\n';
    trace << "fast_path_requires=AVX2 and OS YMM state; guard_failure=original scalar TAC\n";
    for (const auto& f : p.functions) {
        std::vector<Loop> loops = analyze(f,options);
        std::map<int,Plan> plans; std::set<int> fusedSeconds;
        for (size_t i = 0; i < loops.size(); ++i) {
            Loop l = loops[i];
            trace << "function=" << f.name << " loop=" << l.id << " iv=" << l.iv << " start=" << l.start
                  << " bound=" << l.bound << " trip=" << (number(l.bound) ? std::to_string(std::max(0L,std::stol(l.bound)-l.start)) : "assumed") << '\n';
            if (fusedSeconds.count(i)) { trace << "  selected=absorbed_by_fusion (original TAC retained for guard fallback)\n"; continue; }
            if (!l.legal) { trace << "  selected=scalar rejection=" << l.rejection << '\n'; continue; }
            trace << "  dependence=lane-independent; alias=" << (l.guards ? "runtime_disjointness_required" : "proven") << '\n';
            for (const auto& b : l.reads) trace << "  access=read base=" << b << " stride_bytes=4\n";
            for (const auto& b : l.writes) trace << "  access=write base=" << b << " stride_bytes=4\n";
            std::vector<Candidate> choices = alternatives(l,f,options.weights,&trace);
            for (const auto& c : choices) show(trace,c,options.dumpIR);
            Candidate chosen = choices[0];
            if (options.mode == "auto") {
                for (const auto& c : choices) if (c.cost < chosen.cost) chosen = c;
            } else if (options.mode != "scalar") {
                chosen = choices[1];
                for (const auto& c : choices) {
                    if (c.name == "reuse" && (options.mode == "reuse" || options.mode == "fusion" || options.mode == "forward")) chosen = c;
                    if (c.name == "forward" && options.mode == "forward") chosen = c;
                }
            }
            int fusedEnd = -1;
            if (i+1 < loops.size()) {
                std::string reason; std::vector<Node> joined;
                if (fuse(f,l,loops[i+1],joined,reason)) {
                    Loop combined = l;
                    combined.reads.insert(loops[i+1].reads.begin(),loops[i+1].reads.end());
                    combined.writes.insert(loops[i+1].writes.begin(),loops[i+1].writes.end());
                    combined.guards = l.guards || loops[i+1].guards;
                    // Cross-loop aliases also require checks, even if individual loops did not.
                    std::set<std::string> bases = combined.reads; bases.insert(combined.writes.begin(),combined.writes.end());
                    int ptrs = 0; for (const auto& base : bases) if (f.symbols.at(base).pointer()) ++ptrs;
                    combined.guards = ptrs > 1;
                    if (combined.guards && options.alias == "reject") trace << "  fusion_rejected=unknown cross-loop aliases\n";
                    else {
                        Candidate fused = candidate("fusion",joined,combined,f,options.weights);
                        auto nextChoices = alternatives(loops[i+1],f,options.weights);
                        Candidate next = nextChoices[0];
                        for (const auto& c : nextChoices) if (c.cost < next.cost) next = c;
                        show(trace,fused,options.dumpIR);
                        trace << "  fusion_legal=" << reason << "; separate_pair_cost=" << chosen.cost+next.cost << '\n';
                        if (options.mode == "fusion" || (options.mode == "auto" && fused.cost < chosen.cost+next.cost)) {
                            chosen = fused; l = combined; fusedEnd = loops[i+1].end; fusedSeconds.insert(i+1);
                        } else trace << "  fusion_not_selected=" << (options.mode == "auto" ? "unprofitable" : "mode") << '\n';
                    }
                } else trace << "  fusion_rejected=" << reason << '\n';
            }
            trace << "  selected=" << chosen.name << " vector_loads=" << (chosen.name == "scalar" ? 0 : chosen.stats.loads)
                  << " vector_stores=" << (chosen.name == "scalar" ? 0 : chosen.stats.stores) << '\n';
            if (chosen.name != "scalar") {
                Plan plan; plan.loop=l; plan.chosen=chosen; plan.fusedEnd=fusedEnd;
                if (options.mode == "auto" && options.runtimeThreshold == "model")
                    plan.threshold=profitabilityThreshold(l,chosen,f,options.weights);
                trace << "  runtime_threshold=" << plan.threshold
                      << " (0=disabled; actual bound-start below threshold uses original scalar TAC)\n";
                plans[l.header]=plan; anyVectors = true;
            }
        }
        Emitter(f,assembly).emit(plans);
    }
    if (anyVectors) cpuHelper(assembly);
    assembly << ".section .note.GNU-stack,\"\",@progbits\n";
}
int driver(int argc, char** argv) {
    try {
        Options o; std::string input;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--mode" || arg == "--trace" || arg == "--weights" || arg == "--alias-policy" || arg == "--assume-trip" || arg == "--runtime-threshold") {
                if (++i >= argc) throw std::runtime_error("missing option value");
                if (arg == "--mode") o.mode=argv[i];
                if (arg == "--trace") o.trace=argv[i];
                if (arg == "--weights") readWeights(argv[i],o.weights);
                if (arg == "--alias-policy") o.alias=argv[i];
                if (arg == "--runtime-threshold") o.runtimeThreshold=argv[i];
                if (arg == "--assume-trip") {
                    std::string n=argv[i]; if (!number(n) || std::stol(n)<0 || std::stol(n)>536870911) throw std::runtime_error("invalid assumed trip");
                    o.weights.assumedTrip=std::stol(n);
                }
            } else if (arg == "--dump-ir") o.dumpIR = true;
            else if (arg == "--help") {
                std::cout << "Usage: x86_64_backend --mode scalar|simd|reuse|forward|fusion|auto input.tac [--trace file] [--dump-ir] [--weights file] [--assume-trip N] [--alias-policy check|reject] [--runtime-threshold model|off]\n"; return 0;
            } else if (!arg.empty() && arg[0] != '-' && input.empty()) input=arg;
            else throw std::runtime_error("unknown/extra argument: "+arg);
        }
        if (input.empty()) throw std::runtime_error("TAC input required");
        if (o.mode != "scalar" && o.mode != "simd" && o.mode != "reuse" && o.mode != "forward" && o.mode != "fusion" && o.mode != "auto") throw std::runtime_error("invalid optimization mode");
        if (o.alias != "check" && o.alias != "reject") throw std::runtime_error("invalid alias policy");
        if (o.runtimeThreshold != "model" && o.runtimeThreshold != "off") throw std::runtime_error("invalid runtime threshold policy");
        std::ostringstream assembly, trace;
        compile(readProgram(input),o,assembly,trace);
        if (!o.trace.empty()) { std::ofstream file(o.trace); if (!file) throw std::runtime_error("cannot write trace"); file << trace.str(); }
        else std::cerr << trace.str();
        std::cout << assembly.str();
        return 0;
    } catch (const std::exception& e) { std::cerr << "memory backend: " << e.what() << '\n'; return 1; }
}
}
