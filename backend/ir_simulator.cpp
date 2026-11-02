#include "ir.hpp"
#include <iostream>
#include <map>
#include <stdexcept>
#include <cstdlib>

class Simulator {
    std::map<std::string, long long> mem;

    bool isNumber(const std::string& s) const {
        if (s.empty()) return false;
        std::size_t i = (s[0] == '-') ? 1 : 0;
        if (i == s.size()) return false;
        for (; i < s.size(); ++i)
            if (s[i] < '0' || s[i] > '9') return false;
        return true;
    }

    long long value(const std::string& s) const {
        if (s.empty()) return 0;
        if (isNumber(s)) return std::atoll(s.c_str());
        std::map<std::string,long long>::const_iterator it = mem.find(s);
        if (it == mem.end()) return 0;
        return it->second;
    }

public:
    long long run(const std::vector<IRQuad>& code) {
        std::map<std::string,std::size_t> labels;
        std::size_t i;
        for (i = 0; i < code.size(); ++i)
            if (code[i].op == "label") labels[code[i].result] = i;

        for (std::size_t pc = 0; pc < code.size(); ++pc) {
            const IRQuad& q = code[pc];
            if (q.op == "label" || q.op == "func" || q.op == "endfunc" || q.op == "param") continue;
            if (q.op == "=") mem[q.result] = value(q.arg1);
            else if (q.op == "+") mem[q.result] = value(q.arg1) + value(q.arg2);
            else if (q.op == "-") mem[q.result] = value(q.arg1) - value(q.arg2);
            else if (q.op == "*") mem[q.result] = value(q.arg1) * value(q.arg2);
            else if (q.op == "/") {
                long long d = value(q.arg2);
                if (d == 0) throw std::runtime_error("division by zero");
                mem[q.result] = value(q.arg1) / d;
            } else if (q.op == "%") mem[q.result] = value(q.arg1) % value(q.arg2);
            else if (q.op == "if<") { if (value(q.arg1) < value(q.arg2)) pc = labels[q.result] - 1; }
            else if (q.op == "if>") { if (value(q.arg1) > value(q.arg2)) pc = labels[q.result] - 1; }
            else if (q.op == "if<=") { if (value(q.arg1) <= value(q.arg2)) pc = labels[q.result] - 1; }
            else if (q.op == "if>=") { if (value(q.arg1) >= value(q.arg2)) pc = labels[q.result] - 1; }
            else if (q.op == "if==") { if (value(q.arg1) == value(q.arg2)) pc = labels[q.result] - 1; }
            else if (q.op == "if!=") { if (value(q.arg1) != value(q.arg2)) pc = labels[q.result] - 1; }
            else if (q.op == "if") { if (value(q.arg1) != 0) pc = labels[q.result] - 1; }
            else if (q.op == "goto") pc = labels[q.result] - 1;
            else if (q.op == "return") return value(q.arg1);
            else throw std::runtime_error("simulator does not implement " + q.op);
        }
        return 0;
    }
};

int main(int argc, char **argv) {
    std::string path = (argc > 1) ? argv[1] : "sample_tac.txt";
    try {
        std::vector<IRQuad> code = loadTac(path);
        printTac(code);
        Simulator s;
        std::cout << "Return value: " << s.run(code) << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
