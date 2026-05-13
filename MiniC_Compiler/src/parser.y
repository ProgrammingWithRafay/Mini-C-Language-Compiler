%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

extern int yylex();
extern FILE* yyin;
extern int line_no;
extern char* yytext;

ASTNode* root = NULL;

void yyerror(const char* s);

ASTNode* create_node(const char* type, const char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    strcpy(node->type, type);
    if (value != NULL) {
        strcpy(node->value, value);
    } else {
        node->value[0] = '\0';
    }
    node->left = left;
    node->right = right;
    node->next = NULL;
    node->line = line_no;
    return node;
}

void print_ast(ASTNode* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("|-- %s", node->type);
    if (strlen(node->value) > 0) printf(" (%s)", node->value);
    printf("\n");
    
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
    print_ast(node->next, level); // siblings
}

%}

%union {
    int ival;
    char* sval;
    struct ASTNode* node;
}

%expect 1

%token <sval> ID
%token <ival> NUM
%token INT IF ELSE WHILE FOR RETURN PRINTF
%token PLUS MINUS MUL DIV ASSIGN EQ NEQ LT GT LE GE
%token LPAREN RPAREN LBRACE RBRACE SEMI COMMA

%type <node> program decl_list decl var_list stmt_list stmt expr term factor call_args
%type <node> if_stmt while_stmt for_stmt func_decl block

%left EQ NEQ LT GT LE GE
%left PLUS MINUS
%left MUL DIV

%%

program:
    decl_list { root = $1; $$ = $1; }
    ;

decl_list:
    decl decl_list { $1->next = $2; $$ = $1; }
    | decl { $$ = $1; }
    ;

decl:
    func_decl { $$ = $1; }
    | INT var_list SEMI { 
        $$ = create_node("Declaration", "int", $2, NULL); 
    }
    ;

var_list:
    ID ASSIGN expr { 
        ASTNode* id_node = create_node("Identifier", $1, NULL, NULL);
        $$ = create_node("Assignment", "=", id_node, $3);
    }
    | ID { 
        $$ = create_node("Identifier", $1, NULL, NULL);
    }
    | var_list COMMA ID ASSIGN expr {
        ASTNode* id_node = create_node("Identifier", $3, NULL, NULL);
        ASTNode* assign = create_node("Assignment", "=", id_node, $5);
        ASTNode* temp = $1;
        while(temp->next != NULL) temp = temp->next;
        temp->next = assign;
        $$ = $1;
    }
    | var_list COMMA ID {
        ASTNode* id_node = create_node("Identifier", $3, NULL, NULL);
        ASTNode* temp = $1;
        while(temp->next != NULL) temp = temp->next;
        temp->next = id_node;
        $$ = $1;
    }
    ;

func_decl:
    INT ID LPAREN RPAREN block {
        ASTNode* id_node = create_node("Identifier", $2, NULL, NULL);
        $$ = create_node("Function", "int", id_node, $5);
    }
    | INT ID LPAREN INT ID RPAREN block {
        ASTNode* func_id = create_node("Identifier", $2, NULL, NULL);
        ASTNode* param = create_node("Param", $5, NULL, NULL);
        func_id->next = param;
        $$ = create_node("Function", "int", func_id, $7);
    }
    ;

block:
    LBRACE stmt_list RBRACE { $$ = create_node("Block", "", $2, NULL); }
    | LBRACE RBRACE { $$ = create_node("Block", "", NULL, NULL); }
    ;

stmt_list:
    stmt stmt_list { $1->next = $2; $$ = $1; }
    | stmt { $$ = $1; }
    ;

stmt:
    decl { $$ = $1; }
    | ID ASSIGN expr SEMI {
        ASTNode* id_node = create_node("Identifier", $1, NULL, NULL);
        $$ = create_node("Assignment", "=", id_node, $3);
    }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | RETURN expr SEMI { $$ = create_node("Return", "", $2, NULL); }
    | PRINTF LPAREN expr RPAREN SEMI { $$ = create_node("Printf", "", $3, NULL); }
    | block { $$ = $1; }
    ;

if_stmt:
    IF LPAREN expr RPAREN stmt {
        $$ = create_node("If", "", $3, $5);
    }
    | IF LPAREN expr RPAREN stmt ELSE stmt {
        ASTNode* else_node = create_node("Else", "", $7, NULL);
        $5->next = else_node; 
        $$ = create_node("IfElse", "", $3, $5);
    }
    ;

while_stmt:
    WHILE LPAREN expr RPAREN stmt {
        $$ = create_node("While", "", $3, $5);
    }
    ;

for_stmt:
    FOR LPAREN ID ASSIGN expr SEMI expr SEMI ID ASSIGN expr RPAREN block {
        ASTNode* init_id = create_node("Identifier", $3, NULL, NULL);
        ASTNode* init = create_node("Assignment", "=", init_id, $5);
        ASTNode* cond = $7;
        ASTNode* upd_id = create_node("Identifier", $9, NULL, NULL);
        ASTNode* update = create_node("Assignment", "=", upd_id, $11);
        ASTNode* body = $13;

        ASTNode* parts2 = create_node("ForParts2", "", update, body);
        ASTNode* parts1 = create_node("ForParts1", "", cond, parts2);
        $$ = create_node("For", "", init, parts1);
    }
    ;

expr:
    expr PLUS expr { $$ = create_node("BinaryOp", "+", $1, $3); }
    | expr MINUS expr { $$ = create_node("BinaryOp", "-", $1, $3); }
    | expr MUL expr { $$ = create_node("BinaryOp", "*", $1, $3); }
    | expr DIV expr { $$ = create_node("BinaryOp", "/", $1, $3); }
    | expr EQ expr { $$ = create_node("BinaryOp", "==", $1, $3); }
    | expr NEQ expr { $$ = create_node("BinaryOp", "!=", $1, $3); }
    | expr LT expr { $$ = create_node("BinaryOp", "<", $1, $3); }
    | expr GT expr { $$ = create_node("BinaryOp", ">", $1, $3); }
    | expr LE expr { $$ = create_node("BinaryOp", "<=", $1, $3); }
    | expr GE expr { $$ = create_node("BinaryOp", ">=", $1, $3); }
    | term { $$ = $1; }
    ;

term:
    factor { $$ = $1; }
    ;

factor:
    ID { $$ = create_node("Identifier", $1, NULL, NULL); }
    | NUM { 
        char buf[20];
        sprintf(buf, "%d", $1);
        $$ = create_node("Literal", buf, NULL, NULL); 
    }
    | LPAREN expr RPAREN { $$ = $2; }
    | ID LPAREN call_args RPAREN {
        ASTNode* id_node = create_node("Identifier", $1, NULL, NULL);
        $$ = create_node("FunctionCall", "", id_node, $3);
    }
    | ID LPAREN RPAREN {
        ASTNode* id_node = create_node("Identifier", $1, NULL, NULL);
        $$ = create_node("FunctionCall", "", id_node, NULL);
    }
    ;

call_args:
    expr { $$ = $1; }
    | call_args COMMA expr { 
        ASTNode* temp = $1;
        while(temp->next != NULL) temp = temp->next;
        temp->next = $3;
        $$ = $1; 
    }
    ;

%%

void yyerror(const char* s) {
    printf("Syntax Error at line %d: unexpected token %s\n", line_no, yytext);
}
