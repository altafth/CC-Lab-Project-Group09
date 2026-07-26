%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int line_num;
extern int yylex(void);
void yyerror(const char *s);

ASTNode *ast_root = NULL;
int syntax_error_count = 0;
%}

%union {
    int ival;
    float fval;
    char *strval;
    ASTNode *node;
}

%token <ival> INT_LIT BOOL_LIT
%token <fval> FLOAT_LIT
%token <strval> ID
%token INT FLOAT BOOL IF ELSE WHILE PRINT
%token LE GE EQ NE AND OR

%type <node> program stmt_list stmt decl_stmt assign_stmt if_stmt while_stmt print_stmt block expr
%type <ival> type

%left OR
%left AND
%right '!'
%nonassoc '<' '>' LE GE EQ NE
%left '+' '-'
%left '*' '/' '%'
%right UMINUS

%%

program:
      stmt_list { ast_root = make_program($1); }
    ;

stmt_list:
      /* empty */    { $$ = NULL; }
    | stmt_list stmt  { $$ = append_stmt($1, $2); }
    ;

stmt:
      decl_stmt
    | assign_stmt
    | if_stmt
    | while_stmt
    | print_stmt
    | block
    | error ';'  { $$ = NULL; yyerrok; }
    ;

type:
      INT   { $$ = TYPE_INT; }
    | FLOAT { $$ = TYPE_FLOAT; }
    | BOOL  { $$ = TYPE_BOOL; }
    ;

decl_stmt:
      type ID ';'           { $$ = make_decl((DataType)$1, $2, NULL, line_num); }
    | type ID '=' expr ';'  { $$ = make_decl((DataType)$1, $2, $4, line_num); }
    ;

assign_stmt:
      ID '=' expr ';'  { $$ = make_assign($1, $3, line_num); }
    ;

if_stmt:
      IF '(' expr ')' block             { $$ = make_if($3, $5, NULL, line_num); }
    | IF '(' expr ')' block ELSE block  { $$ = make_if($3, $5, $7, line_num); }
    ;

while_stmt:
      WHILE '(' expr ')' block  { $$ = make_while($3, $5, line_num); }
    ;

print_stmt:
      PRINT expr ';'  { $$ = make_print($2, line_num); }
    ;

block:
      '{' stmt_list '}'  { $$ = make_block($2); }
    ;

expr:
      expr '+' expr  { $$ = make_binop("+", $1, $3, line_num); }
    | expr '-' expr  { $$ = make_binop("-", $1, $3, line_num); }
    | expr '*' expr  { $$ = make_binop("*", $1, $3, line_num); }
    | expr '/' expr  { $$ = make_binop("/", $1, $3, line_num); }
    | expr '%' expr  { $$ = make_binop("%", $1, $3, line_num); }
    | expr '<' expr  { $$ = make_binop("<", $1, $3, line_num); }
    | expr '>' expr  { $$ = make_binop(">", $1, $3, line_num); }
    | expr LE expr   { $$ = make_binop("<=", $1, $3, line_num); }
    | expr GE expr   { $$ = make_binop(">=", $1, $3, line_num); }
    | expr EQ expr   { $$ = make_binop("==", $1, $3, line_num); }
    | expr NE expr   { $$ = make_binop("!=", $1, $3, line_num); }
    | expr AND expr  { $$ = make_binop("&&", $1, $3, line_num); }
    | expr OR expr   { $$ = make_binop("||", $1, $3, line_num); }
    | '!' expr              { $$ = make_unop("!", $2, line_num); }
    | '-' expr %prec UMINUS { $$ = make_unop("-", $2, line_num); }
    | '(' expr ')'   { $$ = $2; }
    | ID             { $$ = make_id($1, line_num); }
    | INT_LIT        { $$ = make_int_lit($1, line_num); }
    | FLOAT_LIT      { $$ = make_float_lit($1, line_num); }
    | BOOL_LIT       { $$ = make_bool_lit($1, line_num); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_num, s);
    syntax_error_count++;
}
