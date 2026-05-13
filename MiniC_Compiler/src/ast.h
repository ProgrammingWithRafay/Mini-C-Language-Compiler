#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char type[50];
    char value[50];
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* next;
    int line;
} ASTNode;

void print_ast(ASTNode* node, int level);

#endif
