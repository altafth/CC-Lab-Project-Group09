#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "codegen.h"

static int temp_count = 0, label_count = 0;

static char *new_temp(void) { char b[16]; snprintf(b, sizeof(b), "t%d", ++temp_count); return strdup(b); }
static char *new_label(void) { char b[16]; snprintf(b, sizeof(b), "L%d", ++label_count); return strdup(b); }

static char *gen_expr(ASTNode *n) {
    switch (n->kind) {
        case NODE_INT_LIT: { char *t = new_temp(); printf("%s = %d\n", t, n->ival); return t; }
        case NODE_FLOAT_LIT: { char *t = new_temp(); printf("%s = %.2f\n", t, n->fval); return t; }
        case NODE_BOOL_LIT: { char *t = new_temp(); printf("%s = %s\n", t, n->ival ? "true":"false"); return t; }
        case NODE_ID: return strdup(n->strval);
        case NODE_UNOP: { char *o = gen_expr(n->left); char *t = new_temp(); printf("%s = %s%s\n", t, n->strval, o); return t; }
        case NODE_BINOP: {
            char *l = gen_expr(n->left); char *r = gen_expr(n->right);
            char *t = new_temp(); printf("%s = %s %s %s\n", t, l, n->strval, r); return t;
        }
        default: return strdup("?");
    }
}

static void gen_stmt(ASTNode *n) {
    while (n) {
        switch (n->kind) {
            case NODE_BLOCK: gen_stmt(n->left); break;
            case NODE_DECL:
                if (n->left) { char *v = gen_expr(n->left); printf("%s = %s\n", n->strval, v); }
                break;
            case NODE_ASSIGN: { char *v = gen_expr(n->left); printf("%s = %s\n", n->strval, v); break; }
            case NODE_PRINT: { char *v = gen_expr(n->left); printf("print %s\n", v); break; }
            case NODE_IF: {
                char *c = gen_expr(n->left);
                if (n->third) {
                    char *Le = new_label(), *Lend = new_label();
                    printf("ifFalse %s goto %s\n", c, Le);
                    gen_stmt(n->right);
                    printf("goto %s\n", Lend);
                    printf("%s:\n", Le);
                    gen_stmt(n->third);
                    printf("%s:\n", Lend);
                } else {
                    char *Lend = new_label();
                    printf("ifFalse %s goto %s\n", c, Lend);
                    gen_stmt(n->right);
                    printf("%s:\n", Lend);
                }
                break;
            }
            case NODE_WHILE: {
                char *Ls = new_label(), *Le = new_label();
                printf("%s:\n", Ls);
                char *c = gen_expr(n->left);
                printf("ifFalse %s goto %s\n", c, Le);
                gen_stmt(n->right);
                printf("goto %s\n", Ls);
                printf("%s:\n", Le);
                break;
            }
            default: break;
        }
        n = n->next;
    }
}

void generate_code(ASTNode *root) { gen_stmt(root->left); }