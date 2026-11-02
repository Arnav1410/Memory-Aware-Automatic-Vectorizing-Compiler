#include "compiler.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace memory {
bool number(const std::string& s) {
    if (s.empty()) return false;
    size_t i = s[0] == '-' ? 1 : 0;
    if (i == s.size()) return false;
    for (; i < s.size(); ++i) if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}
static bool identifier(const std::string& s) {
    if (s.empty() || (!std::isalpha(static_cast<unsigned char>(s[0])) && s[0] != '_')) return false;
    for (char c : s) if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') return false;
    return true;
}
Program readProgram(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open " + path);
    Program p;
    std::map<std::string, size_t> index;
    std::string line;
    bool target = false;
    while (std::getline(in, line)) {
        std::istringstream row(line);
        std::string tag, fn, name, type, role;
        row >> tag;
        if (tag == "#@target") {
            int i = 0, ptr = 0; row >> i >> ptr;
            if (i != 4 || ptr != 8) throw std::runtime_error("typed backend requires int32 and 64-bit pointers");
            target = true;
        } else if (tag == "#@function") {
            row >> fn >> type;
            if (!identifier(fn) || index.count(fn)) throw std::runtime_error("invalid/duplicate function metadata");
            if (type != "i32" && type != "void") throw std::runtime_error("unsupported return type " + type);
            index[fn] = p.functions.size();
            Function f; f.name = fn; f.returnType = type; p.functions.push_back(f);
        } else if (tag == "#@symbol") {
            row >> fn >> name >> type >> role;
            if (!index.count(fn) || !identifier(name)) throw std::runtime_error("invalid symbol metadata");
            Symbol s; s.type = type; s.role = role;
            if (type.compare(0, 8, "array32:") == 0) {
                s.type = "array32"; s.count = std::stol(type.substr(8));
                if (s.count <= 0 || s.count > 1048576) throw std::runtime_error("local arrays require 1..1048576 elements");
            } else if (type != "i32" && type != "ptr32") throw std::runtime_error("unsupported symbol type " + type);
            if (role.compare(0, 6, "param:") == 0) {
                s.param = std::stoi(role.substr(6));
                if (s.param < 0 || s.param >= 6 || s.array()) throw std::runtime_error("only six int/pointer register parameters supported");
            } else if (role != "local" && role != "temp") throw std::runtime_error("invalid symbol role");
            Function& f = p.functions[index.at(fn)];
            if (!f.symbols.emplace(name, s).second) throw std::runtime_error("shadowed/colliding TAC name: " + name);
        }
    }
    if (!target || p.functions.empty()) throw std::runtime_error("typed TAC metadata missing; regenerate with tinyc3");
    Function* current = nullptr;
    std::set<std::string> defined;
    for (const IRQuad& q : loadTac(path)) {
        if (q.op == "func") {
            if (current || !index.count(q.arg1) || !defined.insert(q.arg1).second) throw std::runtime_error("bad function boundaries");
            current = &p.functions[index.at(q.arg1)];
        } else if (q.op == "endfunc") {
            if (!current) throw std::runtime_error("unmatched endfunc");
            current = nullptr;
        } else {
            if (!current) throw std::runtime_error("global executable TAC unsupported");
            current->code.push_back(q);
        }
    }
    if (current) throw std::runtime_error("missing endfunc");
    for (const Function& f : p.functions) {
        if (!defined.count(f.name)) throw std::runtime_error("function metadata without body");
        std::set<int> params;
        for (const auto& s : f.symbols) if (s.second.param >= 0) {
            if (!params.insert(s.second.param).second) throw std::runtime_error("duplicate parameter index");
        }
        for (size_t n = 0; n < params.size(); ++n) if (!params.count(n)) throw std::runtime_error("noncontiguous parameters");
        std::set<std::string> labels;
        for (const IRQuad& q : f.code) if (q.op == "label") {
            if (!identifier(q.result) || !labels.insert(q.result).second) throw std::runtime_error("invalid/duplicate label");
        }
        for (const IRQuad& q : f.code) if (q.op == "goto" || q.op.compare(0, 2, "if") == 0)
            if (!labels.count(q.result)) throw std::runtime_error("undefined branch label " + q.result);
    }
    return p;
}
}
