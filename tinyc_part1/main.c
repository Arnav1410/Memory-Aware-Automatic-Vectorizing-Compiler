#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
extern FILE *yyin;
extern char *yytext;
extern int yylineno;
extern const char *tinyc1_current_token_kind(void);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input.c\n", argv[0]);
        return EXIT_FAILURE;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    token_table_init();
    printf("=== Token Stream ===\n");

    int token;
    while ((token = yylex()) != 0) {
        const char *kind = tinyc1_current_token_kind();
        printf("<%s, %s>\n", kind, yytext);
        token_table_add(kind, yytext);
    }

    token_table_print(stdout);
    token_table_free();
    fclose(yyin);
    (void)token;
    return EXIT_SUCCESS;
}
