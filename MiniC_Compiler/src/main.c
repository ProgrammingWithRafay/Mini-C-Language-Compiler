#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"
#include "optimizer.h"
#include "target.h"
#include "interpreter.h"

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern ASTNode* root;
extern int line_no;
extern int semantic_errors;

int phase1 = 1;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    // Phase 1: Lexical Analysis
    printf("===== PHASE 1: LEXICAL ANALYSIS =====\n");
    printf("%-15s | %-15s | %s\n", "Token Value", "Token Type", "Line No.");
    printf("--------------------------------------------------\n");
    yyin = file;
    while(yylex() != 0);

    // Reset for parsing
    fseek(file, 0, SEEK_SET);
    line_no = 1;
    phase1 = 0;

    // Phase 2: Syntax Analysis
    printf("\n===== PHASE 2: SYNTAX ANALYSIS =====\n");
    if (yyparse() == 0) {
        print_ast(root, 0);
    } else {
        return 1;
    }
    printf("\n");

    // Phase 3: Semantic Analysis
    semantic_analysis(root);
    if (semantic_errors > 0) {
        return 1;
    }

    // Phase 4: Intermediate Code Generation
    TAC* tac = generate_tac(root);

    // Phase 5: Optimization
    TAC* opt_tac = optimize_tac(tac);

    // Phase 6: Target Code Generation
    generate_target(opt_tac);

    // Phase 7: Interpreter
    interpret_tac(opt_tac);

    fclose(file);
    return 0;
}
