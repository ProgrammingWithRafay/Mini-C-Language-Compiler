#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "target.h"

void generate_target(TAC* head) {
    printf("===== PHASE 6: TARGET CODE GENERATION =====\n");
    printf("section .data\n");
    
    printf("\nsection .text\n");
    printf("global _main\n\n");
    
    TAC* curr = head;
    while(curr) {
        if(strncmp(curr->op, "func", 4) == 0) {
            printf("_%s:\n", curr->op + 5);
        }
        else if(strcmp(curr->op, "label") == 0) {
            printf("%s:\n", curr->arg1);
        }
        else if(strcmp(curr->op, "=") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  MOV %s, eax\n", curr->res);
        }
        else if(strcmp(curr->op, "+") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  ADD eax, %s\n", curr->arg2);
            printf("  MOV %s, eax\n", curr->res);
        }
        else if(strcmp(curr->op, "-") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  SUB eax, %s\n", curr->arg2);
            printf("  MOV %s, eax\n", curr->res);
        }
        else if(strcmp(curr->op, "*") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  IMUL eax, %s\n", curr->arg2);
            printf("  MOV %s, eax\n", curr->res);
        }
        else if(strcmp(curr->op, "/") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  CDQ\n");
            printf("  IDIV %s\n", curr->arg2);
            printf("  MOV %s, eax\n", curr->res);
        }
        else if(strcmp(curr->op, "iffalse") == 0) {
            printf("  CMP %s, 0\n", curr->arg1);
            printf("  JE %s\n", curr->res);
        }
        else if(strcmp(curr->op, "goto") == 0) {
            printf("  JMP %s\n", curr->arg1);
        }
        else if(strcmp(curr->op, "param") == 0) {
            printf("  PUSH %s\n", curr->arg1);
        }
        else if(strncmp(curr->op, "call", 4) == 0) {
            printf("  CALL _%s\n", curr->op + 5);
            printf("  MOV %s, eax\n", curr->res);
        }
        else if(strcmp(curr->op, "return") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  RET\n");
        }
        else if(strcmp(curr->op, "print") == 0) {
            printf("  PUSH %s\n", curr->arg1);
            printf("  CALL _printf\n");
            printf("  ADD esp, 4\n");
        }
        else if(strcmp(curr->op, "==") == 0 || strcmp(curr->op, "!=") == 0 || 
                strcmp(curr->op, "<") == 0 || strcmp(curr->op, ">") == 0 || 
                strcmp(curr->op, "<=") == 0 || strcmp(curr->op, ">=") == 0) {
            printf("  MOV eax, %s\n", curr->arg1);
            printf("  CMP eax, %s\n", curr->arg2);
            if (strcmp(curr->op, "==") == 0) printf("  SETE al\n");
            if (strcmp(curr->op, "!=") == 0) printf("  SETNE al\n");
            if (strcmp(curr->op, "<") == 0) printf("  SETL al\n");
            if (strcmp(curr->op, ">") == 0) printf("  SETG al\n");
            if (strcmp(curr->op, "<=") == 0) printf("  SETLE al\n");
            if (strcmp(curr->op, ">=") == 0) printf("  SETGE al\n");
            printf("  MOVZX eax, al\n");
            printf("  MOV %s, eax\n", curr->res);
        }
        curr = curr->next;
    }
    printf("\n");
}
