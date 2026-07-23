%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

int line_num = 1;
int lexical_error_count = 0;
%}

%option noyywrap
%x COMMENT

DIGIT     [0-9]
LETTER    [a-zA-Z_]
ID        {LETTER}({LETTER}|{DIGIT})*
INTLIT    {DIGIT}+
FLOATLIT  {DIGIT}+"."{DIGIT}+

%%

"//".*                 { }
"/*"                    { BEGIN(COMMENT); }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>\n             { line_num++; }
<COMMENT>.              { }

[ \t\r]+                { }
\n                       { line_num++; }

"int"     { return INT; }
"float"   { return FLOAT; }
"bool"    { return BOOL; }
"if"      { return IF; }
"else"    { return ELSE; }
"while"   { return WHILE; }
"print"   { return PRINT; }
"true"    { yylval.ival = 1; return BOOL_LIT; }
"false"   { yylval.ival = 0; return BOOL_LIT; }

"<="  { return LE; }
">="  { return GE; }
"=="  { return EQ; }
"!="  { return NE; }
"&&"  { return AND; }
"||"  { return OR; }

{FLOATLIT} { yylval.fval = atof(yytext); return FLOAT_LIT; }
{INTLIT}   { yylval.ival = atoi(yytext); return INT_LIT; }
{ID}       { yylval.strval = strdup(yytext); return ID; }

[+\-*/%<>=!(){};] { return yytext[0]; }

.  { fprintf(stderr, "Lexical Error at line %d: unexpected character '%s'\n", line_num, yytext); lexical_error_count++; }
%%

