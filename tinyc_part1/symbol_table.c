#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

static TokenRecord *head = NULL;
static TokenRecord *tail = NULL;

static char *dup_string(const char *s) {
    const size_t n = strlen(s);
    char *r = (char *)malloc(n + 1);
    if (!r) exit(EXIT_FAILURE);
    memcpy(r, s, n + 1);
    return r;
}

void token_table_init(void) {
    token_table_free();
}

void token_table_add(const char *kind, const char *lexeme) {
    TokenRecord *node = (TokenRecord *)calloc(1, sizeof(TokenRecord));
    if (!node) exit(EXIT_FAILURE);
    node->kind = dup_string(kind);
    node->lexeme = dup_string(lexeme);
    if (!head) head = node;
    else tail->next = node;
    tail = node;
}

void token_table_print(FILE *out) {
    fprintf(out, "\n=== Lexical Symbol Table ===\n");
    fprintf(out, "%-20s | %s\n", "Token", "Lexeme");
    fprintf(out, "---------------------+------------------------------\n");
    for (TokenRecord *p = head; p; p = p->next)
        fprintf(out, "%-20s | %s\n", p->kind, p->lexeme);
}

void token_table_free(void) {
    TokenRecord *p = head;
    while (p) {
        TokenRecord *next = p->next;
        free(p->kind);
        free(p->lexeme);
        free(p);
        p = next;
    }
    head = tail = NULL;
}
