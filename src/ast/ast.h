#ifndef AST_H
#define AST_H

typedef enum {
    TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_UNKNOWN
} DataType;

typedef enum {
    NODE_PROGRAM, NODE_BLOCK, NODE_DECL, NODE_ASSIGN,
    NODE_IF, NODE_WHILE, NODE_PRINT,
    NODE_BINOP, NODE_UNOP, NODE_ID,
    NODE_INT_LIT, NODE_FLOAT_LIT, NODE_BOOL_LIT
} NodeKind;

typedef struct ASTNode {
    NodeKind kind;
    int line;
    DataType eval_type;

    char *strval;
    int ival;
    float fval;

    DataType decl_type;

    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *third;
    struct ASTNode *next;
} ASTNode;

ASTNode *make_program(ASTNode *stmts);
ASTNode *make_block(ASTNode *stmts);
ASTNode *make_decl(DataType type, char *name, ASTNode *init, int line);
ASTNode *make_assign(char *name, ASTNode *expr, int line);
ASTNode *make_if(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line);
ASTNode *make_while(ASTNode *cond, ASTNode *body, int line);
ASTNode *make_print(ASTNode *expr, int line);
ASTNode *make_binop(const char *op, ASTNode *l, ASTNode *r, int line);
ASTNode *make_unop(const char *op, ASTNode *operand, int line);
ASTNode *make_id(char *name, int line);
ASTNode *make_int_lit(int v, int line);
ASTNode *make_float_lit(float v, int line);
ASTNode *make_bool_lit(int v, int line);
ASTNode *append_stmt(ASTNode *list, ASTNode *stmt);

void print_ast(ASTNode *node, int depth);
const char *type_to_str(DataType t);

#endif
