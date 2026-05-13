#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "tac.h"

TAC* generate_tac(ASTNode* node);
void print_tac(TAC* head);

#endif
