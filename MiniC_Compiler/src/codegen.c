#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "tac.h"
#include "codegen.h"

int temp_count = 1;
int label_count = 1;

TAC* tac_head = NULL;
TAC* tac_tail = NULL;

char* new_temp() {
    char* t = (char*)malloc(10);
    sprintf(t, "t%d", temp_count++);
    return t;
}

char* new_label() {
    char* l = (char*)malloc(10);
    sprintf(l, "L%d", label_count++);
    return l;
}

void emit(const char* op, const char* arg1, const char* arg2, const char* res) {
    TAC* inst = (TAC*)malloc(sizeof(TAC));
    strcpy(inst->op, op);
    if(arg1) strcpy(inst->arg1, arg1); else inst->arg1[0] = '\0';
    if(arg2) strcpy(inst->arg2, arg2); else inst->arg2[0] = '\0';
    if(res) strcpy(inst->res, res); else inst->res[0] = '\0';
    inst->next = NULL;

    if (tac_head == NULL) {
        tac_head = tac_tail = inst;
    } else {
        tac_tail->next = inst;
        tac_tail = inst;
    }
}

char* gen_expr(ASTNode* node) {
    if (node == NULL) return "";
    
    if (strcmp(node->type, "Literal") == 0 || strcmp(node->type, "Identifier") == 0) {
        return node->value;
    }
    
    if (strcmp(node->type, "BinaryOp") == 0) {
        char* arg1 = gen_expr(node->left);
        char* arg2 = gen_expr(node->right);
        char* res = new_temp();
        emit(node->value, arg1, arg2, res);
        return res;
    }
    
    if (strcmp(node->type, "FunctionCall") == 0) {
        ASTNode* arg = node->right;
        int p_count = 0;
        char* args[10];
        while(arg) {
            args[p_count++] = gen_expr(arg);
            arg = arg->next;
        }
        for(int i=0; i<p_count; i++) {
            emit("param", args[i], "", "");
        }
        char* res = new_temp();
        char call_str[20];
        sprintf(call_str, "call %s", node->left->value);
        emit(call_str, "", "", res);
        return res;
    }
    return "";
}

void gen_stmt_list(ASTNode* node);

void gen_stmt(ASTNode* node) {
    if (node == NULL) return;

    if (strcmp(node->type, "Function") == 0) {
        char func_label[50];
        sprintf(func_label, "func %s", node->left->value);
        emit(func_label, "", "", "");
        gen_stmt(node->right);
    }
    else if (strcmp(node->type, "Declaration") == 0) {
        ASTNode* var_list = node->left;
        while(var_list) {
            if(strcmp(var_list->type, "Assignment") == 0) {
                char* res = gen_expr(var_list->right);
                emit("=", res, "", var_list->left->value);
            }
            var_list = var_list->next;
        }
    }
    else if (strcmp(node->type, "Assignment") == 0) {
        char* res = gen_expr(node->right);
        emit("=", res, "", node->left->value);
    }
    else if (strcmp(node->type, "If") == 0) {
        char* cond = gen_expr(node->left);
        char* L_end = new_label();
        emit("iffalse", cond, "goto", L_end);
        gen_stmt(node->right);
        emit("label", L_end, "", "");
    }
    else if (strcmp(node->type, "IfElse") == 0) {
        char* cond = gen_expr(node->left);
        char* L_else = new_label();
        char* L_end = new_label();
        emit("iffalse", cond, "goto", L_else);
        
        gen_stmt(node->right);
        emit("goto", L_end, "", "");
        
        emit("label", L_else, "", "");
        if (node->right->next) {
            gen_stmt(node->right->next->left);
        }
        emit("label", L_end, "", "");
    }
    else if (strcmp(node->type, "While") == 0) {
        char* L_start = new_label();
        char* L_end = new_label();
        
        emit("label", L_start, "", "");
        char* cond = gen_expr(node->left);
        emit("iffalse", cond, "goto", L_end);
        
        gen_stmt(node->right);
        emit("goto", L_start, "", "");
        
        emit("label", L_end, "", "");
    }
    else if (strcmp(node->type, "For") == 0) {
        gen_stmt(node->left);
        
        char* L_start = new_label();
        char* L_end = new_label();
        emit("label", L_start, "", "");
        
        ASTNode* parts1 = node->right;
        ASTNode* cond = parts1->left;
        ASTNode* parts2 = parts1->right;
        ASTNode* update = parts2->left;
        ASTNode* body = parts2->right;

        char* cond_res = gen_expr(cond);
        emit("iffalse", cond_res, "goto", L_end);
        
        gen_stmt(body);
        gen_stmt(update);
        
        emit("goto", L_start, "", "");
        emit("label", L_end, "", "");
    }
    else if (strcmp(node->type, "Return") == 0) {
        char* res = gen_expr(node->left);
        emit("return", res, "", "");
    }
    else if (strcmp(node->type, "Printf") == 0) {
        char* res = gen_expr(node->left);
        emit("print", res, "", "");
    }
    else if (strcmp(node->type, "Block") == 0) {
        gen_stmt_list(node->left);
    }
    else {
        gen_stmt(node->left);
        gen_stmt(node->right);
    }
}

void gen_stmt_list(ASTNode* node) {
    while(node != NULL) {
        gen_stmt(node);
        node = node->next;
    }
}

void print_tac(TAC* head) {
    TAC* curr = head;
    while(curr) {
        if(strcmp(curr->op, "label") == 0 || strncmp(curr->op, "func", 4) == 0) {
            if (strncmp(curr->op, "func", 4) == 0) printf("%s:\n", curr->op);
            else printf("%s:\n", curr->arg1);
        }
        else if(strcmp(curr->op, "=") == 0) {
            printf("  %s = %s\n", curr->res, curr->arg1);
        }
        else if(strcmp(curr->op, "iffalse") == 0) {
            printf("  if not %s goto %s\n", curr->arg1, curr->res);
        }
        else if(strcmp(curr->op, "goto") == 0) {
            printf("  goto %s\n", curr->arg1);
        }
        else if(strcmp(curr->op, "param") == 0) {
            printf("  param %s\n", curr->arg1);
        }
        else if(strncmp(curr->op, "call", 4) == 0) {
            printf("  %s = %s\n", curr->res, curr->op);
        }
        else if(strcmp(curr->op, "return") == 0 || strcmp(curr->op, "print") == 0) {
            printf("  %s %s\n", curr->op, curr->arg1);
        }
        else {
            printf("  %s = %s %s %s\n", curr->res, curr->arg1, curr->op, curr->arg2);
        }
        curr = curr->next;
    }
}

TAC* generate_tac(ASTNode* root) {
    printf("===== PHASE 4: INTERMEDIATE CODE GENERATION =====\n");
    gen_stmt_list(root);
    print_tac(tac_head);
    printf("\n");
    return tac_head;
}
