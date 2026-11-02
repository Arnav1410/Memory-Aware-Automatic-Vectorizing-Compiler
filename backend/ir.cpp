#include "ir.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<IRQuad> loadTac(const std::string& path) {
    std::ifstream in(path.c_str());
    if (!in) throw std::runtime_error("cannot open TAC file: " + path);

    std::vector<IRQuad> code;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        std::istringstream ss(line);
        std::vector<std::string> field;
        std::string word;
        while (ss >> word) field.push_back(word);
        if (field.empty()) continue;

        IRQuad q;
        q.op = field[0];
        q.arg1 = "";
        q.arg2 = "";
        q.result = "";

        /* The printed TAC omits empty fields.  Interpret fields according
           to the number of operands of each kind of instruction. */
        if (q.op == "label") {
            if (field.size() >= 2) q.result = field[1];
        } else if (q.op == "goto") {
            if (field.size() >= 2) q.result = field[1];
        } else if (q.op == "return" || q.op == "param") {
            if (field.size() >= 2) q.arg1 = field[1];
        } else if (q.op == "=" || q.op == "=*" || q.op == "=&" ||
                   q.op == "~" || q.op.compare(0, 5, "conv_") == 0 ||
                   ((q.op == "-" || q.op == "+") && field.size() == 3)) {
            if (field.size() >= 2) q.arg1 = field[1];
            if (field.size() >= 3) q.result = field[2];
        } else if (q.op == "if") {
            if (field.size() >= 2) q.arg1 = field[1];
            if (field.size() >= 3) q.arg2 = field[2];
            if (field.size() >= 4) q.result = field[3];
        } else if (q.op.size() > 2 && q.op.compare(0, 2, "if") == 0) {
            if (field.size() >= 2) q.arg1 = field[1];
            if (field.size() >= 3) q.arg2 = field[2];
            if (field.size() >= 4) q.result = field[3];
        } else {
            if (field.size() >= 2) q.arg1 = field[1];
            if (field.size() >= 3) q.arg2 = field[2];
            if (field.size() >= 4) q.result = field[3];
        }

        code.push_back(q);
    }
    return code;
}

void printTac(const std::vector<IRQuad>& code) {
    for (std::size_t i = 0; i < code.size(); ++i) {
        const IRQuad& q = code[i];
        std::cout << i << ": " << q.op << " " << q.arg1 << " "
                  << q.arg2 << " " << q.result << "\n";
    }
}
