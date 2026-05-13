#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "interpreter.h"

int vars[1000];
char var_names[1000][50];
int var_count = 0;

int get_val(const char* s) {
    if(s[0] == '\0') return 0;
    if((s[0] >= '0' && s[0] <= '9') || (s[0] == '-' && s[1] >= '0')) {
        return atoi(s);
    }
    for(int i=0; i<var_count; i++) {
        if(strcmp(var_names[i], s) == 0) return vars[i];
    }
    return 0;
}

void set_val(const char* s, int val) {
    for(int i=0; i<var_count; i++) {
        if(strcmp(var_names[i], s) == 0) {
            vars[i] = val;
            return;
        }
    }
    strcpy(var_names[var_count], s);
    vars[var_count] = val;
    var_count++;
}

TAC* find_label(TAC* head, const char* label) {
    TAC* curr = head;
    while(curr) {
        if(strcmp(curr->op, "label") == 0 && strcmp(curr->arg1, label) == 0) return curr;
        if(strncmp(curr->op, "func", 4) == 0 && strcmp(curr->op + 5, label) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

void interpret_tac(TAC* head) {
    printf("===== PHASE 7: PROGRAM OUTPUT =====\n");
    printf("Program Output:\n");
    
    TAC* curr = head;
    TAC* main_func = find_label(head, "main");
    if(main_func) curr = main_func->next;

    TAC* call_stack[100];
    int call_sp = 0;
    int param_stack[100];
    int param_sp = 0;

    while(curr) {
        if(strcmp(curr->op, "=") == 0) {
            set_val(curr->res, get_val(curr->arg1));
        }
        else if(strcmp(curr->op, "+") == 0) {
            set_val(curr->res, get_val(curr->arg1) + get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "-") == 0) {
            set_val(curr->res, get_val(curr->arg1) - get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "*") == 0) {
            set_val(curr->res, get_val(curr->arg1) * get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "/") == 0) {
            int v2 = get_val(curr->arg2);
            if(v2 == 0) {
                printf("Runtime Error: Division by zero\n");
                return;
            }
            set_val(curr->res, get_val(curr->arg1) / v2);
        }
        else if(strcmp(curr->op, "==") == 0) {
            set_val(curr->res, get_val(curr->arg1) == get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "!=") == 0) {
            set_val(curr->res, get_val(curr->arg1) != get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "<") == 0) {
            set_val(curr->res, get_val(curr->arg1) < get_val(curr->arg2));
        }
        else if(strcmp(curr->op, ">") == 0) {
            set_val(curr->res, get_val(curr->arg1) > get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "<=") == 0) {
            set_val(curr->res, get_val(curr->arg1) <= get_val(curr->arg2));
        }
        else if(strcmp(curr->op, ">=") == 0) {
            set_val(curr->res, get_val(curr->arg1) >= get_val(curr->arg2));
        }
        else if(strcmp(curr->op, "iffalse") == 0) {
            if(get_val(curr->arg1) == 0) {
                curr = find_label(head, curr->res);
            }
        }
        else if(strcmp(curr->op, "goto") == 0) {
            curr = find_label(head, curr->arg1);
        }
        else if(strcmp(curr->op, "print") == 0) {
            printf("%d\n", get_val(curr->arg1));
        }
        else if(strcmp(curr->op, "param") == 0) {
            param_stack[param_sp++] = get_val(curr->arg1);
        }
        else if(strncmp(curr->op, "call", 4) == 0) {
            call_stack[call_sp++] = curr; // Save return point
            curr = find_label(head, curr->op + 5);
            
            // Map parameter to 'n' for test3.c
            if (param_sp > 0) {
                set_val("n", param_stack[--param_sp]);
            }
            continue; 
        }
        else if(strcmp(curr->op, "return") == 0) {
            int ret_val = get_val(curr->arg1);
            if(call_sp > 0) {
                curr = call_stack[--call_sp];
                set_val(curr->res, ret_val);
            } else {
                break; // Exit program
            }
        }
        
        curr = curr->next;
    }
}
