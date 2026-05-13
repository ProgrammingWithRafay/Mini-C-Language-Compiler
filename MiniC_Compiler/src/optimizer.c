#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tac.h"
#include "optimizer.h"
#include "codegen.h"

int is_num(const char* s) {
    if(!s || s[0]=='\0') return 0;
    if(s[0]=='-' && s[1]!='\0') {
        for(int i=1; s[i]; i++) if(!isdigit(s[i])) return 0;
        return 1;
    }
    for(int i=0; s[i]; i++) if(!isdigit(s[i])) return 0;
    return 1;
}

TAC* optimize_tac(TAC* head) {
    printf("===== PHASE 5: CODE OPTIMIZATION =====\n");
    
    // 1. Constant folding
    TAC* curr = head;
    while(curr) {
        if(curr->arg1[0] && curr->arg2[0] && is_num(curr->arg1) && is_num(curr->arg2)) {
            if(strcmp(curr->op, "+")==0 || strcmp(curr->op, "-")==0 || 
               strcmp(curr->op, "*")==0 || strcmp(curr->op, "/")==0) {
                int v1 = atoi(curr->arg1);
                int v2 = atoi(curr->arg2);
                int res = 0;
                if(strcmp(curr->op, "+")==0) res = v1+v2;
                if(strcmp(curr->op, "-")==0) res = v1-v2;
                if(strcmp(curr->op, "*")==0) res = v1*v2;
                if(strcmp(curr->op, "/")==0 && v2!=0) res = v1/v2;
                
                printf("Before Optimization: %s = %s %s %s  ->  After (Constant Folding): %s = %d\n", 
                       curr->res, curr->arg1, curr->op, curr->arg2, curr->res, res);

                sprintf(curr->arg1, "%d", res);
                strcpy(curr->op, "=");
                curr->arg2[0] = '\0';
            }
        }
        curr = curr->next;
    }

    // 2. Dead code elimination
    char used_vars[100][50];
    int use_count = 0;
    
    curr = head;
    while(curr) {
        if(curr->arg1[0]) strcpy(used_vars[use_count++], curr->arg1);
        if(curr->arg2[0]) strcpy(used_vars[use_count++], curr->arg2);
        curr = curr->next;
    }

    TAC* prev = NULL;
    curr = head;
    while(curr) {
        if(curr->res[0] != '\0' && strcmp(curr->op, "=") == 0) {
            int is_used = 0;
            for(int i=0; i<use_count; i++) {
                if(strcmp(curr->res, used_vars[i]) == 0) {
                    is_used = 1; break;
                }
            }
            if(!is_used) {
                printf("Before Optimization: %s = %s  ->  After (Dead Code Elimination): Instruction Removed\n", curr->res, curr->arg1);
                if(prev == NULL) {
                    head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                TAC* temp = curr;
                curr = curr->next;
                free(temp);
                continue;
            }
        }
        prev = curr;
        curr = curr->next;
    }

    printf("\n");
    return head;
}
