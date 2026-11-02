#include "vector_ir.hpp"

#include <iostream>

void VectorProgram::print() const {
    std::cout << "=== Vector IR ===\n";
    std::size_t i;
    for (i = 0; i < code.size(); ++i) {
        const VInstr& v = code[i];
        std::cout << v.op << " " << v.dst << " " << v.a << " " << v.b << '\n';
    }
}
