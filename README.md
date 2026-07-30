# Compiler Construction Lab Project

### Design and Implement a Mini Programming Language Compiler using Flex and Bison

Department of Computer Science and Engineering
Metropolitan University, Bangladesh

---

## About This Repository

This repository is our group's fork of the official Compiler Construction Lab Project template, created in accordance with the Project Manual issued by the course instructor. All implementation, documentation, and testing found here is our own original work, developed individually inside this forked repository as required.

The compiler is written in C, using **Flex** for lexical analysis and **Bison** for syntax analysis, and implements a complete front-end pipeline — from raw source code down to Three Address Code (TAC) — for the fixed mini-language defined in `docs/grammar.md` and in the Project Manual.

---

## Project Status

| Module | Status |
|---|---|
| Lexical Analyzer | ✅ Implemented |
| Syntax Analyzer (CFG + error recovery) | ✅ Implemented |
| Abstract Syntax Tree | ✅ Implemented |
| Symbol Table (nested scopes) | ✅ Implemented |
| Semantic Analyzer | ✅ Implemented |
| Intermediate Code Generation (TAC) | ✅ Implemented |
| Test Suite (valid + lexical/syntax/semantic errors) | ✅ Implemented |
| Documentation (`docs/`) | ✅ In progress |
| Project Report | ⬜ Pending |

---

## Repository Structure

```
CC-Lab-Project/
├── docs/
│   ├── grammar.md
│   ├── architecture.md
│   ├── error_catalog.md
│   ├── design_decisions.md
│   ├── testing_summary.md
│   └── sample_outputs.md
│
├── src/
│   ├── lexer/
│   │   └── lexer.l
│   ├── parser/
│   │   └── parser.y
│   ├── ast/
│   │   ├── ast.h
│   │   └── ast.c
│   ├── semantic/
│   │   ├── semantic.h
│   │   └── semantic.c
│   ├── symbol_table/
│   │   ├── symbol_table.h
│   │   └── symbol_table.c
│   ├── codegen/
│   │   ├── codegen.h
│   │   └── codegen.c
│   └── main.c
│
├── tests/
│   ├── valid_program.mc
│   ├── scope_test.mc
│   ├
│   ├── lexical_error.mc
│   ├── syntax_error.mc
│   ├── semantic_error.mc
│   └── ... (individual per-rule semantic test files)
│
├── examples/
│   └── sample.mc
│
├── build/                  (generated at compile time — not tracked)
├── Makefile
├── .gitignore
└── README.md
```

This follows the directory structure required by the Project Manual (Section 8), with an added `codegen/` module directory for the Intermediate Code Generation phase.

---

## Compiler Pipeline

```
Source Code (.mc)
      |
      v
+--------------------+
|  Lexical Analyzer   |  (Flex)   -->  Token Stream
+--------------------+
      |
      v
+--------------------+
|  Syntax Analyzer    |  (Bison)  -->  Abstract Syntax Tree (AST)
+--------------------+
      |
      v
+--------------------+
| Semantic Analyzer   |  (Symbol Table + Type Checking) --> Validated AST
+--------------------+
      |
      v
+--------------------+
| Intermediate Code   |  (TAC Generator)
|    Generation       |
+--------------------+
      |
      v
Three Address Code (TAC) Output
```

Further architectural detail is documented in [`docs/architecture.md`](docs/architecture.md).

---

## Language Overview

The compiler implements the fixed language specified in the Project Manual:

- **Data types:** `int`, `float`, `bool`
- **Statements:** variable declaration (with or without initializer), assignment, `if` / `if-else`, `while`, `print`, nested `{ }` blocks
- **Operators:** arithmetic (`+ - * / %`), relational (`< > <= >= == !=`), logical (`&& || !`)

The full formal grammar (CFG) is documented in [`docs/grammar.md`](docs/grammar.md).

---

## Build Instructions

Requirements: `flex`, `bison`, `gcc`, `make` (available by default on WSL/Ubuntu; install if missing with `sudo apt install -y flex bison gcc make`).

```bash
make
```

To rebuild from a clean state:

```bash
make clean && make
```

The compiled binary is produced at `build/compiler`.

---

## Execution Instructions

Run the compiler on any `.mc` source file:

```bash
./build/compiler <path-to-source-file>
```

Example:

```bash
./build/compiler examples/sample.mc
```

By default, this prints the full pipeline output: Lexical Analysis, Parsing, AST, Semantic Analysis, Symbol Table, and TAC.

### Selective Phase Output

An optional `--phase=` flag can be passed to isolate the output of a single phase, which was primarily used during development and testing:

```bash
./build/compiler <file>.mc --phase=lexical
./build/compiler <file>.mc --phase=syntax
./build/compiler <file>.mc --phase=ast
./build/compiler <file>.mc --phase=semantic
./build/compiler <file>.mc --phase=symtab
./build/compiler <file>.mc --phase=tac
./build/compiler <file>.mc --phase=all      # same as default
```

Note: the full pipeline always executes internally regardless of this flag, since later phases (e.g. semantic analysis) depend on the AST produced by earlier ones — the flag only filters which phase's output is printed.

---

## Testing

Test programs are organized under `tests/` (error and edge cases, mostly one rule per file) and `examples/` (clean feature demonstrations for the full pipeline). A full index mapping every test file to the specific rule or phase it validates is maintained in [`docs/testing_summary.md`](docs/testing_summary.md), and captured terminal output for key runs is kept in [`docs/sample_outputs.md`](docs/sample_outputs.md).

Coverage includes:

- ✅ Successful compilation of a non-trivial valid program through to TAC
- ✅ Lexical error detection (multiple invalid characters)
- ✅ Syntax error detection with recovery (multiple missing-semicolon errors reported in one run)
- ✅ All six semantic rules from the manual: undeclared variable, redeclaration, scope violation, type mismatch, invalid assignment, invalid expression — tested both individually and combined in a single file

---

## Design Notes

A record of notable design decisions and minor deviations from the manual's minimum grammar (e.g. optional combined declare+initialize syntax, the accepted dangling-else resolution, and the symbol table's scope depth vs. unique-ID choice) is kept in [`docs/design_decisions.md`](docs/design_decisions.md) for reference during viva.

---

## AI Usage Disclosure

AI tools (including Claude) were used during development for guidance, debugging assistance, and documentation drafting, in accordance with the AI Usage Policy stated in the Project Manual and the instructor's template repository. Every group member remains fully able to explain any part of the implementation upon request during demonstration and viva.

---

## Team Members

| Name | Student ID | Contribution |
|---|---|---|
| [Altaf Hosen] | [231-115-108 ] | [Project Setup, lexer, semantic, symbol table, code generation(TAC), Driver/Integration,Documantation.] |
| [Rakibul Islam Rafi] | [231-115-109 ] | [lexer, parser grammar, semantic, symbol table, Testing.] |


---

## Submission

- **Course:** Compiler Construction Lab
- **Institution:** Metropolitan University, Bangladesh
- **Deadline:** 31 July (per Project Manual, Section 16 — no extensions)

---

This repository is a fork of the official instructor-provided template and follows all workflow, structure, and academic integrity requirements stated in the original template README and the Compiler Construction Lab Project Manual.