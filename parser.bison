// Tokens
%token 
  INT  
  PLUS
  MINUS
  TIMES
  DIV
  MOD

// Operator associativity & precedence
%left PLUS MINUS 
%left TIMES DIV MOD

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  Expr* exprValue; 
}

%type <intValue> INT
%type <exprValue> expr

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int eval(Expr *expr);
extern void printExpr(Expr *exp);

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
}

%%
program: /* empty */
       | program expr { 
           // Evaluate and print the result for the expression just parsed
           printf("AST: ");
           printExpr($2);
           printf("\n");
           printf("Result = %d\n", eval($2));
         }
       ;

expr: 
  INT { 
    $$ = ast_integer($1);
  }
  | 
  expr PLUS expr { 
    $$ = ast_operation(PLUS, $1, $3); 
  } 
  |
  expr MINUS expr {
    $$ = ast_operation(MINUS, $1, $3); 
  } 
  |
  expr TIMES expr {
    $$ = ast_operation(TIMES, $1, $3); 
  }
  |
  expr DIV expr {
    $$ = ast_operation(DIV, $1, $3); 
  }
  |
  expr MOD expr {
    $$ = ast_operation(MOD, $1, $3); 
  }
  ;
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

