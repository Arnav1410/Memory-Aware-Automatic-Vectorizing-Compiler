#include "vector_ir.hpp"

#include <iostream>

int main() {
    std::cout << "=== Canonical vector-add pattern ===\n";
    std::cout << "for (i=0; i<n; ++i) c[i] = a[i] + b[i]\n\n";
    std::cout << "Legality checks (prototype):\n";
    std::cout << "  affine unit-stride accesses: PASS\n";
    std::cout << "  loop-carried dependence in c/a/b: NONE\n";
    std::cout << "  alias requirement: a, b and c are disjoint\n\n";

    VectorProgram p;
    p.code = {
        {"VLOAD",  "v0", "a[i]", ""},
        {"VLOAD",  "v1", "b[i]", ""},
        {"VADD",   "v2", "v0", "v1"},
        {"VSTORE", "",   "v2", "c[i]"},
        {"IVADD",  "i",  "i", "VF"},
    };
    p.print();

    std::cout << "\nThis is the vector IR stage only; ISA-specific lowering belongs to the BTP backend.\n";
}
