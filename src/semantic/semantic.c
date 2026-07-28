#include <stdio.h>
#include <string.h>
#include "semantic.h"
#include "symbol_table.h"

static int error_count = 0;

static void sem_error(int line, const char *msg) {
    fprintf(stderr, "Semantic Error at line %d: %s\n", line, msg);
    error_count++;
}

static int is_numeric(DataType t) { return t == TYPE_INT || t == TYPE_FLOAT; }

static DataType check_expr(ASTNode *n) {
    if (!n) return TYPE_UNKNOWN;
    switch (n->kind) {
        case NODE_INT_LIT: n->eval_type = TYPE_INT; break;
        case NODE_FLOAT_LIT: n->eval_type = TYPE_FLOAT; break;
        case NODE_BOOL_LIT: n->eval_type = TYPE_BOOL; break;
        case NODE_ID: {
            Symbol *s = scope_lookup(n->strval);
            if (!s) {
                char m[256]; snprintf(m, sizeof(m), "undeclared variable '%s'", n->strval);
                sem_error(n->line, m); n->eval_type = TYPE_UNKNOWN;
            } else n->eval_type = s->type;
            break;
        }
        case NODE_UNOP: {
            DataType t = check_expr(n->left);
            if (strcmp(n->strval, "!") == 0) {
                if (t != TYPE_BOOL && t != TYPE_UNKNOWN)
                    sem_error(n->line, "logical operator '!' requires a bool operand");
                n->eval_type = TYPE_BOOL;
            } else {
                if (!is_numeric(t) && t != TYPE_UNKNOWN)
                    sem_error(n->line, "unary '-' requires a numeric operand");
                n->eval_type = t;
            }
            break;
        }
        case NODE_BINOP: {
            DataType lt = check_expr(n->left);
            DataType rt = check_expr(n->right);
            const char *op = n->strval;
            if (!strcmp(op,"+")||!strcmp(op,"-")||!strcmp(op,"*")||!strcmp(op,"/")||!strcmp(op,"%")) {
                if ((!is_numeric(lt)&&lt!=TYPE_UNKNOWN)||(!is_numeric(rt)&&rt!=TYPE_UNKNOWN)) {
                    sem_error(n->line, "arithmetic operators require numeric (int/float) operands");
                    n->eval_type = TYPE_UNKNOWN;
                } else n->eval_type = (lt==TYPE_FLOAT||rt==TYPE_FLOAT) ? TYPE_FLOAT : TYPE_INT;
            } else if (!strcmp(op,"<")||!strcmp(op,">")||!strcmp(op,"<=")||!strcmp(op,">=")) {
                if ((!is_numeric(lt)&&lt!=TYPE_UNKNOWN)||(!is_numeric(rt)&&rt!=TYPE_UNKNOWN))
                    sem_error(n->line, "relational operators require numeric (int/float) operands");
                n->eval_type = TYPE_BOOL;
            } else if (!strcmp(op,"==")||!strcmp(op,"!=")) {
                if (lt != rt && lt != TYPE_UNKNOWN && rt != TYPE_UNKNOWN)
                    sem_error(n->line, "cannot compare operands of different types");
                n->eval_type = TYPE_BOOL;
            } else if (!strcmp(op,"&&")||!strcmp(op,"||")) {
                if ((lt!=TYPE_BOOL&&lt!=TYPE_UNKNOWN)||(rt!=TYPE_BOOL&&rt!=TYPE_UNKNOWN))
                    sem_error(n->line, "logical operators require bool operands");
                n->eval_type = TYPE_BOOL;
            }
            break;
        }
        default: break;
    }
    return n->eval_type;
}

static int assignable(DataType target, DataType src) {
    if (target == src) return 1;
    if (target == TYPE_FLOAT && src == TYPE_INT) return 1;
    return 0;
}

static void check_stmt(ASTNode *n);
static void check_list(ASTNode *n) { while (n) { check_stmt(n); n = n->next; } }

static void check_stmt(ASTNode *n) {
    if (!n) return;
    switch (n->kind) {
        case NODE_BLOCK:
            scope_enter(); check_list(n->left); scope_exit();
            break;
        case NODE_DECL: {
            if (!scope_declare(n->strval, n->decl_type, n->line)) {
                char m[256]; snprintf(m, sizeof(m), "redeclaration of variable '%s'", n->strval);
                sem_error(n->line, m);
            }
            if (n->left) {
                DataType it = check_expr(n->left);
                if (it != TYPE_UNKNOWN && !assignable(n->decl_type, it)) {
                    char m[256]; snprintf(m, sizeof(m), "cannot initialize '%s' variable with '%s' expression",
                        type_to_str(n->decl_type), type_to_str(it));
                    sem_error(n->line, m);
                }
            }
            break;
        }
        case NODE_ASSIGN: {
            Symbol *s = scope_lookup(n->strval);
            DataType rt = check_expr(n->left);
            if (!s) {
                char m[256]; snprintf(m, sizeof(m), "assignment to undeclared variable '%s'", n->strval);
                sem_error(n->line, m);
            } else if (rt != TYPE_UNKNOWN && !assignable(s->type, rt)) {
                char m[256]; snprintf(m, sizeof(m), "cannot assign '%s' expression to '%s' variable '%s'",
                    type_to_str(rt), type_to_str(s->type), n->strval);
                sem_error(n->line, m);
            }
            break;
        }
        case NODE_IF: {
            DataType ct = check_expr(n->left);
            if (ct != TYPE_BOOL && ct != TYPE_UNKNOWN) sem_error(n->line, "if condition must be of type bool");
            check_stmt(n->right);
            if (n->third) check_stmt(n->third);
            break;
        }
        case NODE_WHILE: {
            DataType ct = check_expr(n->left);
            if (ct != TYPE_BOOL && ct != TYPE_UNKNOWN) sem_error(n->line, "while condition must be of type bool");
            check_stmt(n->right);
            break;
        }
        case NODE_PRINT: check_expr(n->left); break;
        default: break;
    }
}

int semantic_analyze(ASTNode *root) {
    error_count = 0;
    scope_enter();
    check_list(root->left);
    scope_exit();
    return error_count;
}
