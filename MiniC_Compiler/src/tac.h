#ifndef TAC_H
#define TAC_H

typedef struct TAC {
    char op[10];
    char arg1[50];
    char arg2[50];
    char res[50];
    struct TAC* next;
} TAC;

#endif
