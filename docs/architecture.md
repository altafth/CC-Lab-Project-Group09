# Compiler Architecture

## Pipeline Overview

Source Code (.mc)
      |
      v
+------------------+
|  Lexical Analyzer | (Flex)  -->  Token Stream
+------------------+
      |
      v
+------------------+
|  Syntax Analyzer  | (Bison) -->  Abstract Syntax Tree (AST)
+------------------+
      |
      v
+------------------+
| Semantic Analyzer | (Symbol Table + Type Checking) --> Validated AST
+------------------+
      |
      v
+------------------+
| Intermediate Code | (TAC Generator)
|    Generation     |
+------------------+
      |
      v
Three Address Code (TAC) Output

## Module Responsibilities

| Module | Directory | Responsibility |
|---|---|---|
| Lexer | `src/lexer/` | Converts raw source text into tokens. Discards whitespace/comments. Reports invalid characters. |
| Parser | `src/parser/` | Consumes tokens, validates against CFG, builds AST. Reports syntax errors, recovers using `error` token. |
| AST | `src/ast/` | Node structure for every language construct. Provides text-based tree printer. |
| Symbol Table | `src/symbol_table/` | Tracks declared identifiers via scope-chain (parent-pointer linked list). Supports nested scoping. |
| Semantic Analyzer | `src/semantic/` | Walks AST, checks undeclared variables, redeclaration, scope violations, type mismatches, invalid assignments/expressions. |
| Code Generator | `src/codegen/` | Walks validated AST, emits Three Address Code, handles control flow via labels/jumps. |
| Driver | `src/main.c` | Orchestrates pipeline; supports `--phase=` flag for selective output. |

## Data Flow Between Phases

1. Lexer -> Parser: token stream via on-demand `yylex()` calls.
2. Parser -> Semantic Analyzer: fully-built `ASTNode*` tree (`ast_root`).
3. Semantic Analyzer <-> Symbol Table: `scope_enter()`/`scope_exit()` on Block nodes, `scope_declare()`/`scope_lookup()` on declarations/uses.
4. Semantic Analyzer -> Code Generator: same AST, annotated with `eval_type`.

## Two-Pass Compilation

- Pass 1 (Lexical-only): `while (yylex() != 0)` loop scans the whole file for lexical errors.
- Pass 2 (Parsing): `rewind(yyin)`, then `yyparse()` drives the lexer again on-demand.

This gives Lexical Analysis a distinct, independently-reportable status matching the manual's pipeline diagram.
