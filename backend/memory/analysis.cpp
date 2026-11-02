#include "compiler.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace memory {
namespace {
struct Value {
    std::string base, scalar;
    long k = 0, c = 0;
    bool affine = false;
    int node = -1;
};
std::vector<Node> prune(const std::vector<Node>& input) {
    std::set<int> live;
    for (int i = int(input.size()) - 1; i >= 0; --i) {
        const Node& n = input[i];
        if (n.op == "store" || live.count(i)) {
            live.insert(i);
            if (n.a >= 0) live.insert(n.a);
            if (n.b >= 0) live.insert(n.b);
        }
    }
    std::vector<Node> out;
    std::map<int, int> ids;
    for (int i = 0; i < int(input.size()); ++i) if (live.count(i)) {
        Node n = input[i];
        if (n.a >= 0) n.a = ids.at(n.a);
        if (n.b >= 0) n.b = ids.at(n.b);
        ids[i] = out.size(); out.push_back(n);
    }
    return out;
}
void inspect(const Function& f, Loop& l, const Options& options) {
    const auto& c = f.code;
    const auto& cond = c[l.header + 1];
    l.iv = cond.arg1; l.bound = cond.arg2;
    auto fail = [](const std::string& s) { throw std::runtime_error(s); };
    if (l.header == 0 || c[l.header-1].op != "=" || c[l.header-1].result != l.iv || !number(c[l.header-1].arg1))
        fail("requires immediate constant induction initialization");
    l.start = std::stol(c[l.header-1].arg1);
    if (l.start < 0 || l.start > 536870911) fail("induction start outside safe int32 byte-offset range");
    if (!f.symbols.count(l.iv) || f.symbols.at(l.iv).type != "i32") fail("induction must be int32");
    if (!number(l.bound) && (!f.symbols.count(l.bound) || f.symbols.at(l.bound).type != "i32")) fail("bound must be int32");
    if (number(l.bound) && (std::stol(l.bound) < 0 || std::stol(l.bound) > 536870911)) fail("bound outside supported address range");
    if (l.end < l.body + 3) fail("missing canonical increment");
    const auto& step = c[l.end-3]; const auto& assign = c[l.end-2];
    if (step.op != "+" || step.arg1 != l.iv || step.arg2 != "1" || assign.op != "=" ||
        assign.arg1 != step.result || assign.result != l.iv) fail("requires induction increment of one");
    std::set<std::string> written, addressTaken, assignedInFunction;
    for (const auto& q : c) {
        if (q.op == "=&") addressTaken.insert(q.arg1);
        if (!q.result.empty()) assignedInFunction.insert(q.result);
    }
    if (addressTaken.count(l.iv) || addressTaken.count(l.bound)) fail("address-exposed induction/bound");
    for (int i = l.body; i < l.end-1; ++i) {
        const auto& q = c[i];
        if (!q.result.empty()) written.insert(q.result);
    }
    if (written.count(l.bound)) fail("bound modified in loop");
    for (int i = 0; i < int(c.size()); ++i) {
        const auto& q = c[i];
        if (q.op == "goto" || q.op.compare(0, 2, "if") == 0) {
            for (int j = l.header; j < l.end; ++j) if (c[j].op == "label" && c[j].result == q.result) {
                if (!(i == l.header+1 && j == l.header+3) && !(i == l.end-1 && j == l.header))
                    fail("additional control-flow entry or nested branch");
            }
        }
        if (i >= l.header && i < l.end) continue;
        if (written.count(q.arg1) && q.arg1 != l.iv) fail("loop temporary live outside loop");
        if (written.count(q.arg2) && q.arg2 != l.iv) fail("loop temporary live outside loop");
    }
    std::map<std::string, Value> values;
    for (const auto& s : f.symbols) {
        Value v;
        if (s.second.pointer() && s.second.param >= 0 && !assignedInFunction.count(s.first)) v.base = s.first;
        else if (s.second.type == "i32" && s.second.param >= 0 && !written.count(s.first) && !addressTaken.count(s.first)) v.scalar = s.first;
        else continue;
        values[s.first] = v;
    }
    Value iv; iv.affine = true; iv.k = 1; values[l.iv] = iv;
    auto get = [&](const std::string& name) -> Value {
        if (number(name)) { Value v; v.affine = true; v.c = std::stol(name); return v; }
        if (!values.count(name)) fail("unknown or loop-carried value: " + name);
        return values.at(name);
    };
    auto node = [&](Value v) -> int {
        if (v.node >= 0) return v.node;
        if (!v.base.empty() || (v.affine && v.k != 0)) fail("induction-dependent data expression unsupported");
        if (v.affine) v.scalar = std::to_string(v.c);
        if (v.scalar.empty()) fail("unsupported lane value");
        Node n; n.op = "broadcast"; n.token = v.scalar;
        l.nodes.push_back(n); return l.nodes.size()-1;
    };
    auto address = [&](const Value& v) {
        if (v.base.empty() || v.k != 4 || v.c != 0) fail("memory access is not contiguous base[i] (dependence/stride)");
    };
    for (int i = l.body; i < l.end-3; ++i) {
        const auto& q = c[i]; Value v;
        if (!q.result.empty() && (!f.symbols.count(q.result) || !f.symbols.at(q.result).temp()))
            fail("observable scalar assignment or induction modification");
        if (q.op == "=&") {
            if (!f.symbols.count(q.arg1) || !f.symbols.at(q.arg1).array()) fail("only local array addresses are analyzable");
            v.base = q.arg1;
        } else if (q.op == "=") v = get(q.arg1);
        else if (q.op == "+" || q.op == "-" || q.op == "*") {
            if (q.arg2.empty()) fail("unary expression not vectorized");
            Value a = get(q.arg1), b = get(q.arg2);
            if (q.op == "+" && !a.base.empty() && b.affine) {
                v = a; v.k += b.k; v.c += b.c;
            } else if (a.affine && b.affine) {
                v.affine = true;
                if (q.op == "+") { v.k = a.k+b.k; v.c = a.c+b.c; }
                else if (q.op == "-") { v.k = a.k-b.k; v.c = a.c-b.c; }
                else {
                    if (a.k && b.k) fail("non-affine index");
                    v.k = a.k*b.c+b.k*a.c; v.c = a.c*b.c;
                }
                if (v.k < -536870911 || v.k > 536870911 || v.c < -2147483648L || v.c > 2147483647L)
                    fail("affine arithmetic outside supported range");
            } else {
                Node n; n.op = q.op; n.a = node(a); n.b = node(b);
                v.node = l.nodes.size(); l.nodes.push_back(n);
            }
        } else if (q.op == "=*") {
            Value a = get(q.arg1); address(a); l.reads.insert(a.base);
            Node n; n.op = "load"; n.token = a.base;
            v.node = l.nodes.size(); l.nodes.push_back(n);
        } else if (q.op == "*=") {
            Value a = get(q.arg1); address(a); l.writes.insert(a.base);
            Node n; n.op = "store"; n.token = a.base; n.a = node(get(q.arg2)); l.nodes.push_back(n);
            continue;
        } else fail("unsupported side effect/control/opcode: " + q.op);
        if (q.result.empty()) fail("missing instruction result");
        values[q.result] = v;
    }
    if (l.writes.empty()) fail("no array stores");
    std::set<std::string> bases = l.reads; bases.insert(l.writes.begin(), l.writes.end());
    int pointers = 0;
    for (const auto& base : bases) {
        if (f.symbols.at(base).pointer()) ++pointers;
        if (f.symbols.at(base).array() && number(l.bound) && std::stol(l.bound) > f.symbols.at(base).count)
            fail("constant bound exceeds local array");
    }
    // Local stack objects cannot overlap caller-provided arrays. Pointer pairs need a guard.
    l.guards = pointers > 1;
    if (l.guards && options.alias == "reject") fail("unknown pointer aliasing; runtime checks disabled");
    l.nodes = prune(l.nodes);
    if (statistics(l.nodes).peak > 16) fail("vector register pressure exceeds 16; spill-free subset");
    l.legal = true;
}
}
std::vector<Loop> analyze(const Function& f, const Options& options) {
    std::vector<Loop> out;
    const auto& c = f.code;
    std::map<std::string, int> labels;
    for (int i = 0; i < int(c.size()); ++i) if (c[i].op == "label") labels[c[i].result] = i;
    for (int back = 0; back < int(c.size()); ++back) {
        if (c[back].op != "goto" || !labels.count(c[back].result) || labels.at(c[back].result) >= back) continue;
        Loop l; l.id = out.size(); l.header = labels.at(c[back].result); l.end = back+1; l.body = l.header+4;
        int h = l.header;
        if (h+3 >= back || c[h+1].op != "if<" || c[h+2].op != "goto" || c[h+3].op != "label" ||
            c[h+1].result != c[h+3].result || l.end >= int(c.size()) || c[l.end].op != "label" ||
            c[h+2].result != c[l.end].result) l.rejection = "noncanonical counted loop/control flow";
        else try { inspect(f, l, options); } catch (const std::exception& e) { l.rejection = e.what(); }
        out.push_back(l);
    }
    std::sort(out.begin(), out.end(), [](const Loop& a, const Loop& b) { return a.header < b.header; });
    for (size_t i = 0; i < out.size(); ++i) out[i].id = i;
    return out;
}
std::vector<Node> reuseLoads(const std::vector<Node>& nodes) {
    std::vector<Node> out; std::map<int,int> ids; std::map<std::string,int> available;
    for (int i = 0; i < int(nodes.size()); ++i) {
        Node n = nodes[i];
        if (n.a >= 0) n.a = ids.at(n.a);
        if (n.b >= 0) n.b = ids.at(n.b);
        if (n.op == "load" && available.count(n.token)) { ids[i] = available.at(n.token); continue; }
        ids[i] = out.size();
        if (n.op == "load") available[n.token] = out.size();
        if (n.op == "store") available.erase(n.token); // Same-base writes invalidate cached loads.
        out.push_back(n);
    }
    return prune(out);
}
bool validNodes(const std::vector<Node>& nodes, std::string& reason) {
    for (int i = 0; i < int(nodes.size()); ++i) {
        const Node& n = nodes[i];
        bool binary = n.op == "+" || n.op == "-" || n.op == "*";
        bool store = n.op == "store";
        bool leaf = n.op == "load" || n.op == "broadcast";
        auto value = [&](int id) { return id >= 0 && id < i && nodes[id].op != "store"; };
        if ((!binary && !store && !leaf) ||
            ((binary || store) ? !value(n.a) : n.a != -1) ||
            (binary ? !value(n.b) : n.b != -1) ||
            ((leaf || store) && n.token.empty())) {
            reason = "unsupported opcode or invalid earlier-value reference at node " + std::to_string(i);
            return false;
        }
    }
    reason.clear(); return true;
}
Forwarding forwardStoredValues(const Loop& loop, const std::vector<Node>& nodes) {
    Forwarding result; result.nodes = nodes;
    if (!loop.legal) { result.rejection = "requires analyzed lane-independent base[i] loop"; return result; }
    if (!validNodes(nodes,result.rejection)) return result;
    for (const auto& n : nodes) {
        if ((n.op == "load" && !loop.reads.count(n.token)) ||
            (n.op == "store" && !loop.writes.count(n.token))) {
            result.rejection = "memory base outside analyzed access proof"; return result;
        }
    }
    std::vector<Node> out;
    std::vector<int> ids(nodes.size(),-1);
    // Scoped to ONE vector body. Other bases are disjoint only under the original
    // loop proof/entry guards. Unknown side effects were rejected above.
    std::map<std::string,int> stored;
    for (int i = 0; i < int(nodes.size()); ++i) {
        Node n = nodes[i];
        if (n.a >= 0) n.a = ids.at(n.a);
        if (n.b >= 0) n.b = ids.at(n.b);
        if (n.op == "load" && stored.count(n.token)) {
            ids[i] = stored.at(n.token); ++result.eliminated; continue;
        }
        ids[i] = out.size();
        // Keep every store in place; a later store supersedes the earlier value.
        if (n.op == "store") stored[n.token] = n.a;
        out.push_back(n);
    }
    result.nodes = prune(out);
    if (!validNodes(result.nodes,result.rejection))
        throw std::logic_error("invalid forwarding result: " + result.rejection);
    return result;
}
Stats statistics(const std::vector<Node>& nodes) {
    Stats s; std::vector<int> uses(nodes.size());
    for (const Node& n : nodes) { if (n.a >= 0) ++uses[n.a]; if (n.b >= 0) ++uses[n.b]; }
    int live = 0;
    for (int i = 0; i < int(nodes.size()); ++i) {
        const Node& n = nodes[i];
        if (n.op == "load") ++s.loads;
        else if (n.op == "store") ++s.stores;
        else if (n.op == "broadcast") ++s.broadcasts;
        else { ++s.alu; if (n.op == "*") ++s.multiplies; }
        if (n.op != "store") { ++live; s.peak = std::max(s.peak, live); }
        if (n.a >= 0 && --uses[n.a] == 0) --live;
        if (n.b >= 0 && --uses[n.b] == 0) --live;
        if (n.op != "store" && uses[i] == 0) --live;
    }
    return s;
}
bool fuse(const Function& f, const Loop& a, const Loop& b, std::vector<Node>& nodes, std::string& reason) {
    auto reject = [&](const std::string& s) { reason = s; return false; };
    if (!a.legal || !b.legal) return reject("one loop failed dependence analysis");
    if (b.header != a.end+2 || a.iv != b.iv || a.start != b.start || a.bound != b.bound)
        return reject("iteration domains/order are incompatible or loops are not adjacent");
    if (a.writes.size() != 1) return reject("producer must write exactly one temporary");
    std::string temp = *a.writes.begin();
    if (!f.symbols.at(temp).array()) return reject("producer output is not an internal array");
    if (a.reads.count(temp) || !b.reads.count(temp) || b.writes.count(temp)) return reject("temporary has unsupported dependencies");
    int stores = 0;
    for (const Node& n : a.nodes) if (n.op == "store") ++stores;
    if (stores != 1) return reject("producer must define each temporary element once");
    for (const auto& base : b.writes) if (a.reads.count(base)) return reject("consumer overwrites producer input");
    for (int i = 0; i < int(f.code.size()); ++i) {
        const auto& q = f.code[i];
        if (q.arg1 != temp && q.arg2 != temp && q.result != temp) continue;
        if (i < a.body || i >= b.end || (i >= a.end && i < b.body)) return reject("temporary escapes or is used outside loop pair");
        if (q.op != "=&" || q.arg1 != temp) return reject("temporary is not exclusively addressed locally");
    }
    // Single-entry region: no branch from elsewhere may bypass the second initialization.
    for (int i = 0; i < int(f.code.size()); ++i) if (i < a.header || i > b.end) {
        const auto& q = f.code[i];
        if ((q.op == "goto" || q.op.compare(0, 2, "if") == 0) && q.result == f.code[a.end].result)
            return reject("external branch into loop pair");
    }
    nodes.clear(); std::map<int,int> first, second; int produced = -1;
    for (int i = 0; i < int(a.nodes.size()); ++i) {
        Node n = a.nodes[i];
        if (n.a >= 0) n.a = first.at(n.a);
        if (n.b >= 0) n.b = first.at(n.b);
        if (n.op == "store") { produced = n.a; continue; }
        first[i] = nodes.size(); nodes.push_back(n);
    }
    for (int i = 0; i < int(b.nodes.size()); ++i) {
        Node n = b.nodes[i];
        if (n.op == "load" && n.token == temp) { second[i] = produced; continue; }
        if (n.a >= 0) n.a = second.at(n.a);
        if (n.b >= 0) n.b = second.at(n.b);
        second[i] = nodes.size(); nodes.push_back(n);
    }
    nodes = reuseLoads(nodes);
    if (statistics(nodes).peak > 16) return reject("fused vector register pressure exceeds 16");
    reason = "internal temporary " + temp + " has no escaping or later observable use";
    return true;
}
}
