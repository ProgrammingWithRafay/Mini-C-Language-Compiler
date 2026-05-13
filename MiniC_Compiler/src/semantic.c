#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"

typedef struct {
    char name[50];
    char type[50];
    char scope[50];
    int line;
    int is_func;
} Symbol;

Symbol sym_tab[100];
int sym_count = 0;
int semantic_errors = 0;
char current_scope[50] = "global";

void add_symbol(const char* name, const char* type, int line, int is_func) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(sym_tab[i].name, name) == 0 && strcmp(sym_tab[i].scope, current_scope) == 0) {
            printf("Semantic Error at line %d: Duplicate declaration of '%s'\n", line, name);
            semantic_errors++;
            return;
        }
    }
    strcpy(sym_tab[sym_count].name, name);
    strcpy(sym_tab[sym_count].type, type);
    strcpy(sym_tab[sym_count].scope, current_scope);
    sym_tab[sym_count].line = line;
    sym_tab[sym_count].is_func = is_func;
    sym_count++;
}

int check_declared(const char* name, int line) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(sym_tab[i].name, name) == 0 && 
            (strcmp(sym_tab[i].scope, current_scope) == 0 || strcmp(sym_tab[i].scope, "global") == 0)) {
            return 1;
        }
    }
    printf("Semantic Error at line %d: Undeclared variable '%s'\n", line, name);
    semantic_errors++;
    return 0;
}

void traverse_ast(ASTNode* node) {
    if (node == NULL) return;

    if (strcmp(node->type, "Function") == 0) {
        add_symbol(node->left->value, "int", node->line, 1);
        strcpy(current_scope, node->left->value);
        if (node->left->next != NULL && strcmp(node->left->next->type, "Param") == 0) {
            add_symbol(node->left->next->value, "int", node->line, 0);
        }
        traverse_ast(node->right);
        strcpy(current_scope, "global");
    } 
    else if (strcmp(node->type, "Declaration") == 0) {
        ASTNode* var_list = node->left;
        while (var_list != NULL) {
            if (strcmp(var_list->type, "Assignment") == 0) {
                add_symbol(var_list->left->value, "int", node->line, 0);
                traverse_ast(var_list->right);
            } else if (strcmp(var_list->type, "Identifier") == 0) {
                add_symbol(var_list->value, "int", node->line, 0);
            }
            var_list = var_list->next;
        }
    }
    else if (strcmp(node->type, "Identifier") == 0) {
        check_declared(node->value, node->line);
    }
    else if (strcmp(node->type, "Assignment") == 0) {
        check_declared(node->left->value, node->line);
        traverse_ast(node->right);
    }
    else if (strcmp(node->type, "FunctionCall") == 0) {
        check_declared(node->left->value, node->line);
        traverse_ast(node->right);
    }
    else {
        traverse_ast(node->left);
        traverse_ast(node->right);
    }
    
    traverse_ast(node->next);
}

void semantic_analysis(ASTNode* root) {
    printf("===== PHASE 3: SEMANTIC ANALYSIS =====\n");
    traverse_ast(root);

    printf("%-15s | %-15s | %-15s | %-10s | %s\n", "Variable", "Type", "Scope", "Line Decl.", "Status");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < sym_count; i++) {
        printf("%-15s | %-15s | %-15s | %-10d | %s\n", 
               sym_tab[i].name, 
               sym_tab[i].is_func ? "function(int)" : "int", 
               sym_tab[i].scope, 
               sym_tab[i].line, 
               "Valid");
    }

    if (semantic_errors == 0) {
        printf("\nSemantic Analysis Passed\n\n");
    } else {
        printf("\nSemantic Analysis Failed with %d errors\n\n", semantic_errors);
    }
}
