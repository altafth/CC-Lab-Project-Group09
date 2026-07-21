%{
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"

int yylex(void);
void yyerror(const char *s);
extern int line_num;

ASTNode *ast_root = NULL;
int syntax_error_count = 0;
%}
