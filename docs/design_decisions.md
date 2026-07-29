
# Design Decisions and Deviations

## 1. Combined Declaration + Initialization
Manual's grammar implies separate `int x; x = 5;`. This compiler also supports `int x = 5;` because the manual's own semantic-error example (Section 4.5) uses this form (`bool b = 5 + 3.2;`). Both forms supported.

## 2. Dangling-Else Resolution
One shift/reduce conflict for `if-else`. Bison's default shift resolution (bind `else` to nearest unmatched `if`) is accepted as-is.

## 3. Scope Representation: Depth vs. Unique ID
Symbol table's Scope field shows nesting depth (0=global, 1=nested, 2=deep nested), not a unique block ID. Sibling blocks at same depth show same number. `scope_lookup()` correctness is unaffected since it works via parent pointers, not the displayed number.

## 4. Selective Phase Output (--phase= flag)
`src/main.c` accepts `--phase=lexical|syntax|ast|semantic|symtab|tac|all`. Full pipeline always runs internally; the flag only filters printed output.

## 5. Symbol Table Log vs. Live Scope Structure
Live scope-chain (`Scope`/`Symbol`) is freed on `scope_exit()` to enforce correct scoping. A separate append-only log (`SymLog`) is never freed, purely so `print_symbol_table()` can show every variable ever declared, including from closed scopes. No effect on semantic correctness.
