
# Formal Context-Free Grammar (CFG)

## Notation
- `::=` means "is defined as"
- `|` means "or" (alternative)

## Grammar Rules

program        ::= stmt_list

stmt_list      ::= /* empty */
                 | stmt_list stmt

stmt           ::= decl_stmt
                 | assign_stmt
                 | if_stmt
                 | while_stmt
                 | print_stmt
                 | block

type           ::= 'int' | 'float' | 'bool'

decl_stmt      ::= type ID ';'
                 | type ID '=' expr ';'

assign_stmt    ::= ID '=' expr ';'

if_stmt        ::= 'if' '(' expr ')' block
                 | 'if' '(' expr ')' block 'else' block

while_stmt     ::= 'while' '(' expr ')' block

print_stmt     ::= 'print' expr ';'

block          ::= '{' stmt_list '}'

expr           ::= expr '+' expr
                 | expr '-' expr
                 | expr '*' expr
                 | expr '/' expr
                 | expr '%' expr
                 | expr '<' expr
                 | expr '>' expr
                 | expr '<=' expr
                 | expr '>=' expr
                 | expr '==' expr
                 | expr '!=' expr
                 | expr '&&' expr
                 | expr '||' expr
                 | '!' expr
                 | '-' expr
                 | '(' expr ')'
                 | ID
                 | INT_LIT
                 | FLOAT_LIT
                 | BOOL_LIT

## Operator Precedence (lowest to highest)

| Precedence | Operators | Associativity |
|---|---|---|
| 1 (lowest) | `||` | Left |
| 2 | `&&` | Left |
| 3 | `!` (unary) | Right |
| 4 | `<` `>` `<=` `>=` `==` `!=` | Non-associative |
| 5 | `+` `-` (binary) | Left |
| 6 | `*` `/` `%` | Left |
| 7 (highest) | `-` (unary/UMINUS) | Right |

## Known Grammar Ambiguity

if (a) if (b) print x; else print y;

Dangling-else conflict, resolved by Bison's default shift behavior (binds `else` to nearest unmatched `if`).

## Lexical Tokens

| Token | Pattern |
|---|---|
| `INT`, `FLOAT`, `BOOL` | keywords `int`, `float`, `bool` |
| `IF`, `ELSE`, `WHILE`, `PRINT` | keywords |
| `ID` | `[a-zA-Z_][a-zA-Z0-9_]*` |
| `INT_LIT` | `[0-9]+` |
| `FLOAT_LIT` | `[0-9]+\.[0-9]+` |
| `BOOL_LIT` | `true`, `false` |
| `LE, GE, EQ, NE, AND, OR` | `<=`, `>=`, `==`, `!=`, `&&`, `||` |
| single-char tokens | `+ - * / % < > = ! ( ) { } ;` |
