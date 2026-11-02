#include "ast.hpp"

void print_indent(FILE* out, int depth) {
    int i;
    for (i = 0; i < depth; ++i) std::fprintf(out, "  ");
}

ASTNode* ast_node(const std::string& label) {
    return new ASTNode(label);
}

ASTNode* ast_leaf(const std::string& kind, const std::string& text) {
    return ast_node(kind + ": " + text);
}

void ast_add(ASTNode* parent, ASTNode* child) {
    if (parent != NULL && child != NULL) parent->children.push_back(child);
}

void ast_print(const ASTNode* root, FILE* out, int depth) {
    if (root == NULL) return;
    print_indent(out, depth);
    std::fprintf(out, "%s\n", root->label.c_str());
    std::size_t i;
    for (i = 0; i < root->children.size(); ++i)
        ast_print(root->children[i], out, depth + 1);
}

void ast_free(ASTNode* root) {
    if (root == NULL) return;
    std::size_t i;
    for (i = 0; i < root->children.size(); ++i)
        ast_free(root->children[i]);
    delete root;
}
