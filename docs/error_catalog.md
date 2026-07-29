
# Error Message Catalog

## Lexical Errors
| Trigger | Message Format |
|---|---|
| Unrecognized character | `Lexical Error at line X: unexpected character 'c'` |

## Syntax Errors
| Trigger | Message Format |
|---|---|
| Any grammar violation | `Syntax Error at line X: syntax error` |

Recovery: `stmt: ... | error ';'` skips to the next `;` and continues, allowing multiple syntax errors in one run.

## Semantic Errors
| Rule | Message Format | Example Trigger |
|---|---|---|
| Undeclared variable | `undeclared variable 'name'` | using `x` before declaring it |
| Redeclaration | `redeclaration of variable 'name'` | declaring `int x;` twice in same scope |
| Scope violation | `undeclared variable 'name'` / `assignment to undeclared variable 'name'` | using variable outside its block |
| Type mismatch (declaration) | `cannot initialize 'type' variable with 'type' expression` | `bool b = 5 + 3.2;` |
| Type mismatch (assignment) | `cannot assign 'type' expression to 'type' variable 'name'` | assigning `float` to `bool` |
| Invalid assignment | `cannot assign 'bool' expression to 'int' variable 'name'` | `int a; a = flag;` |
| Invalid arithmetic expr | `arithmetic operators require numeric (int/float) operands` | `+` on `bool` operands |
| Invalid relational expr | `relational operators require numeric (int/float) operands` | `<` on non-numeric operands |
| Invalid logical expr | `logical operators require bool operands` | `x && y` where x,y are int |
| Invalid comparison | `cannot compare operands of different types` | `int == bool` |
| Invalid condition | `if condition must be of type bool` / `while condition must be of type bool` | `if (5) { ... }` |

All semantic errors are collected and reported together (analyzer does not stop at first error).
