#ifndef TINYC1_SYMBOL_TABLE_H
#define TINYC1_SYMBOL_TABLE_H

#include <stdio.h>

typedef struct TokenRecord {
    char *kind;
    char *lexeme;
    struct TokenRecord *next;
} TokenRecord;

void token_table_init(void);
void token_table_add(const char *kind, const char *lexeme);
void token_table_print(FILE *out);
void token_table_free(void);

#endif
