#pragma once

#include <string>
#include <vector>
#include <iostream>

struct IRQuad {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

std::vector<IRQuad> loadTac(const std::string& path);
void printTac(const std::vector<IRQuad>& code);
