#include "ast.hpp"

#include <cstdio>
#include <cstdlib>

extern int yyparse(void);
extern FILE* yyin;
extern ASTNode* tinyc2_root;
extern int yylineno;

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::fprintf(stderr, "Usage: %s input.c [output.txt]\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = std::fopen(argv[1], "r");
    if (yyin == NULL) {
        std::perror(argv[1]);
        return EXIT_FAILURE;
    }

    int rc = yyparse();
    std::fclose(yyin);
    if (rc != 0 || tinyc2_root == NULL) {
        std::fprintf(stderr, "Parse failed near line %d\n", yylineno);
        return EXIT_FAILURE;
    }

    FILE* out = stdout;
    if (argc == 3) {
        out = std::fopen(argv[2], "w");
        if (out == NULL) {
            std::perror(argv[2]);
            ast_free(tinyc2_root);
            tinyc2_root = NULL;
            return EXIT_FAILURE;
        }
    }

    ast_print(tinyc2_root, out);

    if (out != stdout) std::fclose(out);
    ast_free(tinyc2_root);
    tinyc2_root = NULL;
    return EXIT_SUCCESS;
}
