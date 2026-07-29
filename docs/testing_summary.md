
# Testing Summary

This document indexes every test program under `tests/` and `examples/`, mapping each to the specific rule or phase it validates, per the requirements of Section 15 of the Project Manual.

## Valid / Full-Pipeline Programs

| File | Purpose | Expected Result |
|---|---|---|
| `examples/sample.mc` | Full pipeline demo (while, if-else, all 3 data types) | Compiles cleanly through to TAC |
| `tests/valid_program.mc` | Full pipeline demo (while loop, arithmetic) | Compiles cleanly through to TAC |
| `tests/scope_test.mc` | Nested block, variable shadowing scope | Compiles cleanly; symbol table shows two scope levels |
| `tests/all_types_scopes.mc` | int/float/bool across global, nested, and doubly-nested scopes | Compiles cleanly; symbol table shows all types and scope depths |

## Lexical Error Tests

| File | Purpose | Expected Result |
|---|---|---|
| `tests/lex_invalid_char.mc` | Single invalid character (`$`) | `Lexical Analysis: FAILED (1 lexical error(s) found)` |
| `tests/lexical_all_errors.mc` | Multiple invalid characters (`$ @ # ~`) | `Lexical Analysis: FAILED (4 lexical error(s) found)`, one message per bad character |

## Syntax Error Tests

| File | Purpose | Expected Result |
|---|---|---|
| `tests/syntax_missing_semicolon.mc` | Missing `;` after an assignment | One `Syntax Error at line X` |
| `tests/syntax_all_errors.mc` | Multiple missing semicolons | Multiple `Syntax Error` lines, demonstrating `error ';'` recovery |

## Semantic Error Tests (one rule per file)

| File | Rule Tested | Expected Result |
|---|---|---|
| `tests/semantic_undeclared.mc` | Undeclared variable use | `undeclared variable 'name'` |
| `tests/semantic_redeclaration.mc` | Redeclaration in same scope | `redeclaration of variable 'name'` |
| `tests/semantic_scope_violation.mc` | Use outside declaring block | `undeclared variable 'name'` (scope-based) |
| `tests/semantic_type_mismatch.mc` | Assigning wrong type on init | `cannot initialize 'type' variable with 'type' expression` |
| `tests/semantic_invalid_assignment.mc` | Assigning bool to int variable | `cannot assign 'bool' expression to 'int' variable 'name'` |
| `tests/semantic_invalid_expression.mc` | Logical op on non-bool operands | `logical operators require bool operands` |

## Combined Semantic Test

| File | Purpose | Expected Result |
|---|---|---|
| `tests/semantic_all_errors.mc` | All 6 semantic rules triggered in a single file | 6 semantic errors reported together, followed by `Compilation failed: 6 semantic error(s).` |

## How to Run

```bash
./build/compiler examples/sample.mc
./build/compiler tests/<filename>.mc
./build/compiler tests/<filename>.mc --phase=lexical|syntax|ast|semantic|symtab|tac
```
