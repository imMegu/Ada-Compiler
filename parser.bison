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

%token WHILE IF THEN ELSE ASSIGN SEMICOLON LBRACE RBRACE LPAREN RPAREN
%token ID // For identifiers/variables

// Solve ELSE conflict
%expect 1

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
  char *stringValue;
// ID for token
  Cmd* cmdValue;
}

%type <intValue> INT
%type <stringValue> ID
%type <exprValue> expr
%type <boolExprValue> boolExpr
%type <cmdValue> cmd
%type <cmdValue> cmd_sequence
%type <cmdValue> stmt
%type <cmdValue> program

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int eval(Expr *expr);
int evalBool(BoolExpr *expr);
int execCmd(Cmd *cmd);
extern void printExpr(Expr *exp);
extern void printBool(BoolExpr *exp);
extern void printCmd(Cmd *cmd);

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
}

%%
program:
  /* empty */
  { $$ = NULL; }
  | program stmt SEMICOLON {
    $$ = ast_sequence($1, $2);
  }
  | program stmt {
    $$ = ast_sequence($1, $2);
  }
  ;

stmt:
  cmd { 
    $$ = $1;
    printf("CMD AST: ");
    printCmd($1);
    printf("\n");
    execCmd($1);
  }
  |
  expr {
    printf("AST: ");
    printExpr($1);
    printf("\n");
    printf("Result = %d\n", eval($1));
    $$ = NULL;
  }
  |
  boolExpr {
    printf("Bool AST: ");
    printBool($1);
    printf("\n");
    printf("Bool Result = %d\n", evalBool($1));
    $$ = NULL;
  }
  ;

cmd_sequence:
  /* empty */
  { $$ = NULL; }
  | cmd_sequence cmd SEMICOLON {
    $$ = ($1 == NULL) ? $2 : ast_sequence($1, $2);
  }
  | cmd_sequence cmd {
    // This allows for 'cmd1 cmd2' if you don't require SEMICOLON
    $$ = ($1 == NULL) ? $2 : ast_sequence($1, $2);
  }
  ;

cmd: 
  ID ASSIGN expr {
    // x = 5 + y
    $$ = ast_assign($1, $3);
    // Cleanup the string value from the ID token
    free($1); 
  }
  |
  IF LPAREN boolExpr RPAREN cmd %prec THEN {
    // if ( B ) cmd
    $$ = ast_if($3, $5, NULL);
  }
  |
  IF LPAREN boolExpr RPAREN cmd ELSE cmd {
    // if ( B ) cmd1 else cmd2
    $$ = ast_if($3, $5, $7);
  }
  |
  WHILE LPAREN boolExpr RPAREN cmd {
    // while ( B ) cmd
    $$ = ast_while($3, $5);
  }
  |
  LPAREN cmd RPAREN {
    // Grouping of commands: ( c1 ; c2 )
    $$ = $2;
  }
  |
  LBRACE cmd_sequence RBRACE {
    // Command block: { cmd1 ; cmd2 ... }
    // The 'program' non-terminal handles the sequence of commands
    $$ = $2;
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
  ID {
    $$ = ast_variable($1);
    free($1);
  }
  |
  LPAREN expr RPAREN {
    // Expression grouping: ( expr )
    $$ = $2;
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
