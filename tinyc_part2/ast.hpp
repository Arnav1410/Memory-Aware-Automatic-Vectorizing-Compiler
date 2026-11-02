#pragma once

#include <cstdio>
#include <string>
#include <vector>

struct ASTNode {
    std::string label;
    std::vector<ASTNode*> children;
    ASTNode(const std::string& text) : label(text) {}
};

ASTNode* ast_node(const std::string& label);
ASTNode* ast_leaf(const std::string& kind, const std::string& text);
void ast_add(ASTNode* parent, ASTNode* child);
void ast_print(const ASTNode* root, FILE* out, int depth = 0);
void ast_free(ASTNode* root);
