
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "semantic.h"
#include "codegen.h"

extern FILE *yyin;
extern int yyparse(void);
extern int yylex(void);
extern int line_num;
extern ASTNode *ast_root;
extern int syntax_error_count;
extern int lexical_error_count;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source-file> [--phase=lexical|syntax|ast|semantic|symtab|tac]\n", argv[0]);
        return 1;
    }

    /* কোন phase দেখাতে হবে, default = all (পুরো pipeline, presentation demo এর জন্য) */
    const char *phase = "all";
    if (argc >= 3 && strncmp(argv[2], "--phase=", 8) == 0) {
        phase = argv[2] + 8;
    }

    int show_lexical  = !strcmp(phase, "all") || !strcmp(phase, "lexical");
    int show_syntax   = !strcmp(phase, "all") || !strcmp(phase, "syntax");
    int show_ast      = !strcmp(phase, "all") || !strcmp(phase, "ast");
    int show_semantic = !strcmp(phase, "all") || !strcmp(phase, "semantic");
    int show_symtab   = !strcmp(phase, "all") || !strcmp(phase, "symtab");
    int show_tac      = !strcmp(phase, "all") || !strcmp(phase, "tac");

    yyin = fopen(argv[1], "r");
    if (!yyin) { fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]); return 1; }

    /* ---- Phase 1: Lexical Analysis ---- */
    if (show_lexical) printf("===== Lexical Analysis =====\n");
    while (yylex() != 0) { }

    if (show_lexical) {
        if (lexical_error_count == 0)
            printf("Lexical Analysis: SUCCESS (no lexical errors)\n");
        else
            printf("Lexical Analysis: FAILED (%d lexical error(s) found)\n", lexical_error_count);
    }

    if (lexical_error_count > 0) {
        printf("\nCompilation failed: %d lexical error(s).\n", lexical_error_count);
        fclose(yyin);
        return 1;
    }

    rewind(yyin);
    line_num = 1;

    /* ---- Phase 2: Syntax Analysis ---- */
    if (show_syntax) printf("\n===== Parsing =====\n");
    yyparse();
    fclose(yyin);

    if (syntax_error_count > 0) {
        printf("\nCompilation failed: %d syntax error(s).\n", syntax_error_count);
        return 1;
    }
    if (show_syntax) printf("Parsing successful.\n");

    /* ---- Phase 3: AST ---- */
    if (show_ast) {
        printf("\n===== Abstract Syntax Tree =====\n");
        print_ast(ast_root, 0);
    }

    /* ---- Phase 4: Semantic Analysis ---- */
    if (show_semantic) printf("\n===== Semantic Analysis =====\n");
    int sem_errors = semantic_analyze(ast_root);

    /* ---- Phase 5: Symbol Table ---- */
    if (show_symtab) {
        printf("\n===== Symbol Table =====\n");
        print_symbol_table();
    }

    if (sem_errors > 0) {
        printf("\nCompilation failed: %d semantic error(s).\n", sem_errors);
        return 1;
    }
    if (show_semantic) printf("\nSemantic analysis successful. No errors found.\n");

    /* ---- Phase 6: TAC ---- */
    if (show_tac) {
        printf("\n===== Three Address Code (TAC) =====\n");
        generate_code(ast_root);
    }

    return 0;
}
