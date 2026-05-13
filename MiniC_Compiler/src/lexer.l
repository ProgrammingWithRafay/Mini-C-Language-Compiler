%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

extern int phase1;
int line_no = 1;

void print_token(const char* val, const char* type) {
    if(phase1) {
        printf("%-15s | %-15s | %d\n", val, type, line_no);
    }
}
%}

%option noyywrap nounput noinput

%%

"int"       { print_token(yytext, "KEYWORD"); return INT; }
"if"        { print_token(yytext, "KEYWORD"); return IF; }
"else"      { print_token(yytext, "KEYWORD"); return ELSE; }
"while"     { print_token(yytext, "KEYWORD"); return WHILE; }
"for"       { print_token(yytext, "KEYWORD"); return FOR; }
"return"    { print_token(yytext, "KEYWORD"); return RETURN; }
"printf"    { print_token(yytext, "KEYWORD"); return PRINTF; }

[0-9]+      { 
                print_token(yytext, "INTEGER_LITERAL"); 
                yylval.ival = atoi(yytext); 
                return NUM; 
            }

[a-zA-Z_][a-zA-Z0-9_]* { 
                print_token(yytext, "IDENTIFIER"); 
                yylval.sval = strdup(yytext); 
                return ID; 
            }

"+"         { print_token(yytext, "OPERATOR"); return PLUS; }
"-"         { print_token(yytext, "OPERATOR"); return MINUS; }
"*"         { print_token(yytext, "OPERATOR"); return MUL; }
"/"         { print_token(yytext, "OPERATOR"); return DIV; }
"="         { print_token(yytext, "OPERATOR"); return ASSIGN; }
"=="        { print_token(yytext, "OPERATOR"); return EQ; }
"!="        { print_token(yytext, "OPERATOR"); return NEQ; }
"<"         { print_token(yytext, "OPERATOR"); return LT; }
">"         { print_token(yytext, "OPERATOR"); return GT; }
"<="        { print_token(yytext, "OPERATOR"); return LE; }
">="        { print_token(yytext, "OPERATOR"); return GE; }

"("         { print_token(yytext, "PUNCTUATION"); return LPAREN; }
")"         { print_token(yytext, "PUNCTUATION"); return RPAREN; }
"{"         { print_token(yytext, "PUNCTUATION"); return LBRACE; }
"}"         { print_token(yytext, "PUNCTUATION"); return RBRACE; }
";"         { print_token(yytext, "PUNCTUATION"); return SEMI; }
","         { print_token(yytext, "PUNCTUATION"); return COMMA; }

"//".*      { /* single line comment, do nothing */ }
[ \t\r]+    { /* ignore whitespace */ }
\n          { line_no++; }

.           { printf("Lexical Error at line %d: unknown character %s\n", line_no, yytext); }

%%
