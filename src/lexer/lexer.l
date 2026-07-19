%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.tab.h"

int line_num = 1;
%}

%%
"int"                { return INT; }
"float"              { return FLOAT; }
"bool"               { return BOOL; }
"if"                 { return IF; }
"else"               { return ELSE; }
"while"              { return WHILE; }
"print"              { return PRINT; }
"true"               { yylval.bval = 1; return BOOLLIT; }
"false"              { yylval.bval = 0; return BOOLLIT; }

[0-9]+"."[0-9]+      { yylval.fval = atof(yytext); return FLOATLIT; }
[0-9]+               { yylval.ival = atoi(yytext); return INTLIT; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }

"=="                 { return EQ; }
"!="                 { return NEQ; }
"<="                 { return LE; }
">="                 { return GE; }
"&&"                 { return AND; }
"||"                 { return OR; }
"!"                  { return NOT; }

[-+*/%<>=(){};]      { return yytext[0]; }

"//".*               { /* single-line comment, discard */ }
"/*"([^*]|\*+[^*/])*\*+"/"  { /* block comment, discard */ }

[ \t\r]+             { /* whitespace, discard */ }
\n                   { line_num++; }

.                    { printf("Lexical Error: invalid token '%s' at line %d\n", yytext, line_num); }
%%
