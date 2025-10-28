// Tokens
%token 
  INT  
  PLUS
  MINUS
  TIMES
  DIV
  MOD
  EQ // ==
  NE // !=
  LT // <
  GT // >
  LE // <=
  GE // >=

// Operator associativity & precedence
// lowest -> highest
%left EQ NE LT GT LE GE
%left PLUS MINUS 
%left TIMES DIV MOD

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  Expr* exprValue;
  BoolExpr* boolExprValue;
}

%type <intValue> INT
%type <exprValue> expr
%type <boolExprValue> boolExpr

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int eval(Expr *expr);
int evalBool(BoolExpr *expr);
extern void printExpr(Expr *exp);
extern void printBool(BoolExpr *exp);

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
}

%%
program: /* empty */
       |
program statement { }
       ;

statement:
  expr { 
    // Action for arithmetic expression
    printf("AST: ");
    printExpr($1);
    printf("\n");
    printf("Result = %d\n", eval($1));
  }
  |
  boolExpr {
    // Action for boolean expression. eval() is for Expr, so we need a new evalBool()
    printf("Bool AST: ");
    printBool($1);
    printf("\n");
    printf("Bool Result = %d\n", evalBool($1)); 
  }
  ;

boolExpr:
  expr EQ expr {
    $$ = ast_relation(EQ, $1, $3);
  }
  |
  expr NE expr {
    $$ = ast_relation(NE, $1, $3);
  }
  |
  expr LT expr {
    $$ = ast_relation(LT, $1, $3);
  }
  |
  expr GT expr {
    $$ = ast_relation(GT, $1, $3);
  }
  |
  expr LE expr {
    $$ = ast_relation(LE, $1, $3);
  }
  |
  expr GE expr {
    $$ = ast_relation(GE, $1, $3);
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

