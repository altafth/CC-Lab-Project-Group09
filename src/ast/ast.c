#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *new_node(NodeKind kind, int line) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    n->kind = kind;
    n->line = line;
    n->eval_type = TYPE_UNKNOWN;
    return n;
}

ASTNode *append_stmt(ASTNode *list, ASTNode *stmt) {
    if (!stmt) return list;
    if (!list) return stmt;
    ASTNode *cur = list;
    while (cur->next) cur = cur->next;
    cur->next = stmt;
    return list;
}

ASTNode *make_program(ASTNode *stmts) {
    ASTNode *n = new_node(NODE_PROGRAM, 0);
    n->left = stmts;
    return n;
}

ASTNode *make_block(ASTNode *stmts) {
    ASTNode *n = new_node(NODE_BLOCK, 0);
    n->left = stmts;
    return n;
}

ASTNode *make_decl(DataType type, char *name, ASTNode *init, int line) {
    ASTNode *n = new_node(NODE_DECL, line);
    n->decl_type = type;
    n->strval = name;
    n->left = init;
    return n;
}

ASTNode *make_assign(char *name, ASTNode *expr, int line) {
    ASTNode *n = new_node(NODE_ASSIGN, line);
    n->strval = name;
    n->left = expr;
    return n;
}

ASTNode *make_if(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line) {
    ASTNode *n = new_node(NODE_IF, line);
    n->left = cond; n->right = then_b; n->third = else_b;
    return n;
}

ASTNode *make_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = new_node(NODE_WHILE, line);
    n->left = cond; n->right = body;
    return n;
}

ASTNode *make_print(ASTNode *expr, int line) {
    ASTNode *n = new_node(NODE_PRINT, line);
    n->left = expr;
    return n;
}

ASTNode *make_binop(const char *op, ASTNode *l, ASTNode *r, int line) {
    ASTNode *n = new_node(NODE_BINOP, line);
    n->strval = strdup(op); n->left = l; n->right = r;
    return n;
}

ASTNode *make_unop(const char *op, ASTNode *operand, int line) {
    ASTNode *n = new_node(NODE_UNOP, line);
    n->strval = strdup(op); n->left = operand;
    return n;
}

ASTNode *make_id(char *name, int line) {
    ASTNode *n = new_node(NODE_ID, line);
    n->strval = name;
    return n;
}

ASTNode *make_int_lit(int v, int line) {
    ASTNode *n = new_node(NODE_INT_LIT, line);
    n->ival = v; n->eval_type = TYPE_INT;
    return n;
}

ASTNode *make_float_lit(float v, int line) {
    ASTNode *n = new_node(NODE_FLOAT_LIT, line);
    n->fval = v; n->eval_type = TYPE_FLOAT;
    return n;
}

ASTNode *make_bool_lit(int v, int line) {
    ASTNode *n = new_node(NODE_BOOL_LIT, line);
    n->ival = v; n->eval_type = TYPE_BOOL;
    return n;
}

const char *type_to_str(DataType t) {
    switch (t) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_BOOL: return "bool";
        default: return "unknown";
    }
}

static void indent(int d) { for (int i = 0; i < d; i++) printf("  "); }

static void print_list(ASTNode *n, int depth) {
    while (n) { print_ast(n, depth); n = n->next; }
}

void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    indent(depth);
    switch (node->kind) {
        case NODE_PROGRAM: printf("Program\n"); print_list(node->left, depth+1); break;
        case NODE_BLOCK: printf("Block\n"); print_list(node->left, depth+1); break;
        case NODE_DECL:
            printf("Decl (%s %s) [line %d]\n", type_to_str(node->decl_type), node->strval, node->line);
            if (node->left) print_ast(node->left, depth+1);
            break;
        case NODE_ASSIGN:
            printf("Assign (%s) [line %d]\n", node->strval, node->line);
            print_ast(node->left, depth+1);
            break;
        case NODE_IF:
            printf("If [line %d]\n", node->line);
            indent(depth+1); printf("Cond:\n"); print_ast(node->left, depth+2);
            indent(depth+1); printf("Then:\n"); print_ast(node->right, depth+2);
            if (node->third) { indent(depth+1); printf("Else:\n"); print_ast(node->third, depth+2); }
            break;
        case NODE_WHILE:
            printf("While [line %d]\n", node->line);
            indent(depth+1); printf("Cond:\n"); print_ast(node->left, depth+2);
            indent(depth+1); printf("Body:\n"); print_ast(node->right, depth+2);
            break;
        case NODE_PRINT:
            printf("Print [line %d]\n", node->line);
            print_ast(node->left, depth+1);
            break;
        case NODE_BINOP:
            printf("BinOp (%s) [line %d]\n", node->strval, node->line);
            print_ast(node->left, depth+1); print_ast(node->right, depth+1);
            break;
        case NODE_UNOP:
            printf("UnOp (%s) [line %d]\n", node->strval, node->line);
            print_ast(node->left, depth+1);
            break;
        case NODE_ID: printf("Id (%s) [line %d]\n", node->strval, node->line); break;
        case NODE_INT_LIT: printf("IntLit (%d) [line %d]\n", node->ival, node->line); break;
        case NODE_FLOAT_LIT: printf("FloatLit (%.2f) [line %d]\n", node->fval, node->line); break;
        case NODE_BOOL_LIT: printf("BoolLit (%s) [line %d]\n", node->ival ? "true":"false", node->line); break;
    }
}
