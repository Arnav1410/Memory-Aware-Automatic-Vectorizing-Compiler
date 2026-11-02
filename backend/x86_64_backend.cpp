#include "../common/target_info.hpp"
#include "ir.hpp"
#include "memory/compiler.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>

using tinyc::TargetInfo;

class X86Backend {
    TargetInfo target;
    std::map<std::string, std::ptrdiff_t> slot;
    std::size_t frameBytes;

    bool isNumber(const std::string& s) const {
        if (s.empty()) return false;
        std::size_t i = (s[0] == '-') ? 1 : 0;
        if (i == s.size()) return false;
        for (; i < s.size(); ++i)
            if (!std::isdigit((unsigned char)s[i])) return false;
        return true;
    }

    std::string suffix() const { return target.intSize == 4 ? "l" : "q"; }
    std::string regA() const { return target.intSize == 4 ? "%eax" : "%rax"; }
    std::string regC() const { return target.intSize == 4 ? "%ecx" : "%rcx"; }

    std::ptrdiff_t alloc(const std::string& name) {
        std::map<std::string,std::ptrdiff_t>::iterator it = slot.find(name);
        if (it != slot.end()) return it->second;
        frameBytes += target.intSize;
        std::ptrdiff_t off = -(std::ptrdiff_t)frameBytes;
        slot[name] = off;
        return off;
    }

    void load(const std::string& x, const std::string& reg) {
        if (isNumber(x)) std::cout << "    mov" << suffix() << " $" << x << ", " << reg << "\n";
        else std::cout << "    mov" << suffix() << " " << alloc(x) << "(%rbp), " << reg << "\n";
    }

    void store(const std::string& reg, const std::string& x) {
        std::cout << "    mov" << suffix() << " " << reg << ", " << alloc(x) << "(%rbp)\n";
    }

public:
    X86Backend(const TargetInfo& t) : target(t), frameBytes(0) {}

    void emit(const std::vector<IRQuad>& code) {
        if (target.intSize != 4 && target.intSize != 8)
            throw std::runtime_error("educational x86 backend expects 4- or 8-byte int target");

        for (std::size_t i = 0; i < code.size(); ++i) {
            const IRQuad& q = code[i];
            if (q.op == "label" || q.op == "func" || q.op == "endfunc" || q.op == "param") continue;
            if (!q.arg1.empty() && !isNumber(q.arg1) &&
                q.op != "goto" && q.op != "label")
                alloc(q.arg1);
            if (!q.arg2.empty() && !isNumber(q.arg2)) alloc(q.arg2);
            if (!q.result.empty() && q.result[0] != '?' &&
                q.op != "goto" && q.op != "label" &&
                q.op.compare(0, 2, "if") != 0)
                alloc(q.result);
        }

        std::size_t align = target.stackAlignment;
        if (align == 0) align = 1;
        if (frameBytes % align != 0) frameBytes += align - (frameBytes % align);

        std::cout << ".text\n.globl main\nmain:\n";
        std::cout << "    pushq %rbp\n    movq %rsp, %rbp\n";
        if (frameBytes) std::cout << "    subq $" << frameBytes << ", %rsp\n";

        for (std::size_t i = 0; i < code.size(); ++i) {
            const IRQuad& q = code[i];
            if (q.op == "label") { std::cout << q.result << ":\n"; continue; }
            if (q.op == "func" || q.op == "endfunc" || q.op == "param") continue;

            if (q.op == "=") {
                load(q.arg1, regA());
                store(regA(), q.result);
            } else if (q.op == "+" || q.op == "-" || q.op == "*") {
                load(q.arg1, regA()); load(q.arg2, regC());
                if (q.op == "+") std::cout << "    add" << suffix() << " " << regC() << ", " << regA() << "\n";
                else if (q.op == "-") std::cout << "    sub" << suffix() << " " << regC() << ", " << regA() << "\n";
                else std::cout << "    imul" << suffix() << " " << regC() << ", " << regA() << "\n";
                store(regA(), q.result);
            } else if (q.op == "/") {
                load(q.arg1, regA()); load(q.arg2, regC());
                if (target.intSize == 4) std::cout << "    cltd\n    idivl %ecx\n";
                else std::cout << "    cqto\n    idivq %rcx\n";
                store(regA(), q.result);
            } else if (q.op == "return") {
                if (!q.arg1.empty()) load(q.arg1, regA());
                else std::cout << "    xor" << suffix() << " " << regA() << ", " << regA() << "\n";
                std::cout << "    jmp .Ltinyc_return\n";
            } else if (q.op == "goto") {
                std::cout << "    jmp " << q.result << "\n";
            } else if (q.op == "if") {
                load(q.arg1, regA());
                std::cout << "    cmp" << suffix() << " $0, " << regA() << "\n";
                std::cout << "    jne " << q.result << "\n";
            } else if (q.op.compare(0, 2, "if") == 0) {
                load(q.arg1, regA()); load(q.arg2, regC());
                std::cout << "    cmp" << suffix() << " " << regC() << ", " << regA() << "\n";
                std::string cmp = q.op.substr(2);
                std::string j;
                if (cmp == "<") j = "jl";
                else if (cmp == ">") j = "jg";
                else if (cmp == "<=") j = "jle";
                else if (cmp == ">=") j = "jge";
                else if (cmp == "==") j = "je";
                else if (cmp == "!=") j = "jne";
                else throw std::runtime_error("bad comparison");
                std::cout << "    " << j << " " << q.result << "\n";
            } else {
                throw std::runtime_error("backend does not implement " + q.op);
            }
        }

        /* If control reaches the end without an explicit return, use 0. */
        std::cout << "    xor" << suffix() << " " << regA() << ", " << regA() << "\n";
        std::cout << "    jmp .Ltinyc_return\n";
        std::cout << ".Ltinyc_return:\n";
        std::cout << "    leave\n    ret\n";
        std::cout << "\n.section .note.GNU-stack,\"\",@progbits\n";
    }
};

int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--mode" || std::string(argv[i]) == "--help")
            return memory::driver(argc, argv);
    try {
        std::string tac = argc > 1 ? argv[1] : "sample_tac.txt";
        std::string cfg = argc > 2 ? argv[2] : "../targets/x86_64_tinyc.conf";
        TargetInfo target = TargetInfo::fromConfigFile(cfg);
        std::vector<IRQuad> code = loadTac(tac);
        X86Backend backend(target);
        backend.emit(code);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
