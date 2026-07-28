#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

typedef struct Scope {
    Symbol *symbols;
    struct Scope *parent;
    int level;
} Scope;

typedef struct SymLog {
    char *name;
    DataType type;
    int scope_level;
    int line_declared;
    struct SymLog *next;
} SymLog;

static Scope *current = NULL;
static SymLog *log_head = NULL;
static SymLog *log_tail = NULL;

/* depth-based scope: global = 0, প্রতিটা nested block এ ঢুকলে +1 */
void scope_enter(void) {
    Scope *s = malloc(sizeof(Scope));
    s->symbols = NULL;
    s->parent = current;
    s->level = current ? current->level + 1 : 0;
    current = s;
}

void scope_exit(void) {
    if (!current) return;
    Symbol *sym = current->symbols;
    while (sym) { Symbol *t = sym; sym = sym->next; free(t->name); free(t); }
    Scope *parent = current->parent;
    free(current);
    current = parent;
}

int scope_declare(const char *name, DataType type, int line) {
    for (Symbol *s = current->symbols; s; s = s->next)
        if (strcmp(s->name, name) == 0) return 0;

    Symbol *s = malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->type = type;
    s->scope_level = current->level;
    s->line_declared = line;
    s->next = current->symbols;
    current->symbols = s;

    SymLog *e = malloc(sizeof(SymLog));
    e->name = strdup(name);
    e->type = type;
    e->scope_level = current->level;
    e->line_declared = line;
    e->next = NULL;
    if (!log_head) log_head = log_tail = e;
    else { log_tail->next = e; log_tail = e; }

    return 1;
}

Symbol *scope_lookup(const char *name) {
    for (Scope *sc = current; sc; sc = sc->parent)
        for (Symbol *s = sc->symbols; s; s = s->next)
            if (strcmp(s->name, name) == 0) return s;
    return NULL;
}

void print_symbol_table(void) {
    if (!log_head) { printf("(empty - no variables declared)\n"); return; }

    printf("%-10s%-9s%-9s%s\n", "Name", "Type", "Scope", "Line Declared");
    printf("-----------------------------------------\n");

    for (SymLog *e = log_head; e; e = e->next) {
        printf("%-10s%-9s%-9d%d\n", e->name, type_to_str(e->type), e->scope_level, e->line_declared);
    }
}
