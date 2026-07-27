#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "ast.h"

typedef struct Symbol {
    char *name;
    DataType type;
    int scope_level;
    int line_declared;
    struct Symbol *next;
} Symbol;

void scope_enter(void);
void scope_exit(void);
int scope_declare(const char *name, DataType type, int line);
Symbol *scope_lookup(const char *name);
void print_symbol_table(void);

#endif
