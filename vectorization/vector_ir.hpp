#pragma once

#include <string>
#include <vector>

struct VInstr {
    std::string op;
    std::string dst;
    std::string a;
    std::string b;
};

struct VectorProgram {
    std::vector<VInstr> code;
    void print() const;
};
