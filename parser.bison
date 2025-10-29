// Tokens
%token 
  INT  
  STR
  PLUS
  MINUS
  MULT
  DIV
  MOD
  EQ NE LT GT LE GE
  TRUE FALSE INT_TYPE BOOL_TYPE
%token
  WHILE  // WHILE
  IF     // IF
  THEN   // THEN
  ELSE   // ELSE
  LOOP   // LOOP
  END    // END
  ASSIGN // :=
  SEMICOLON
  LPAREN 
  RPAREN
%token 
  ID // For identifiers/variables
  PROCEDURE IS MAIN COLON COMMA BEG
  GET_LINE PUT_LINE

// Operator associativity & precedence
// lowest -> highest
%left EQ NE LT GT LE GE
%left PLUS MINUS 
%left MULT DIV MOD

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.

%union {
  int intValue;
  int typeKind;
  char *idValue, *strValue;
  Expr* exprValue;
  Command* cmdValue;
  LinkedList* cmdList;
  LinkedList* idList;
}

%type <intValue> INT
%type <idValue> ID
%type <strValue> STR
%type <exprValue> expr
%type <cmdValue> cmd
%type <cmdList> cmd_list
%type <idList> id_list
%type <idList> decl
%type <idList> decls
%type <typeKind> type

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "linkedlist.h"
#include "symboltable.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);

extern Command *root;
}

%%
program: 
  PROCEDURE MAIN IS { symtab_enter_scope(); } decls BEG cmd_list END MAIN SEMICOLON {
    root = ast_compound($7); // $7 is cmd_list now
    symtab_exit_scope(); // Exit global scope after parsing
  }

decls:
  decl decls {
    $$ = concatLists($1, $2);
  }
  | 
  decl {
    $$ = $1;
  }
  | 
  {
    $$ = mkEmptyList();
  }
  ;

decl:
  id_list COLON type SEMICOLON {
    // Iterate over the list of IDs ($1) and insert each into the symbol table
    LinkedList *ids = $1;
    Node *current = ids->first;
    while (current != NULL) {
      char *var_name = (char *)current->value;
      if (symtab_insert(var_name, $3 == TYPE_INTEGER ? TYPE_INT : TYPE_BOOL) != 0) {
        // Error handling for re-declaration (optional, but good practice)
        yyerror("Variable already declared in this scope");
      }
      current = current->next;
    }

    $$ = $1;
  }

type:
  INT_TYPE {
    $$ = TYPE_INTEGER; // Returns an integer constant/enum value
  }
  |
  BOOL_TYPE {
    $$ = TYPE_BOOLEAN; // Returns a different integer constant/enum value
  }
  ;
id_list:
  ID {
    $$ = mkList($1);
  }
  | 
  ID COMMA id_list {
    $$ = concatStart($1, $3);
  }
  ;

cmd_list:
  cmd cmd_list {
    $$ = concatStart($1, $2);
  }
  |
  cmd {
    $$ = mkList($1);
  }
  |
  {
    $$ = mkEmptyList();
  }
  ;

cmd:
  ID ASSIGN expr SEMICOLON {
    if (symtab_lookup($1) == NULL) { // Check if ID is declared
      yyerror("Undeclared variable in assignment");
    }
    $$ = ast_expr(ast_assignment($1, $3));
  }
  |
  IF expr THEN cmd_list END IF SEMICOLON {
    Command *body = ast_compound($4);
    $$ = ast_if($2, body);
  }
  |  
  IF expr THEN cmd_list ELSE cmd_list END IF SEMICOLON {
    Command *if_body = ast_compound($4);
    Command *else_body = ast_compound($6);
    $$ = ast_if_then_else($2, if_body, else_body);
  }
  |
  WHILE expr LOOP cmd_list END LOOP SEMICOLON {
    Command *body = ast_compound($4);
    $$ = ast_while($2, body);
  }
  |
  PUT_LINE expr SEMICOLON {
    $$ = ast_put_line($2);
  }
  |
  GET_LINE ID SEMICOLON {
    if (symtab_lookup($2) == NULL) { // Check if ID is declared
      yyerror("Undeclared variable in Get_Line");
    }
    $$ = ast_get_line($2);
  }
 ;
expr:
  INT {
    $$ = ast_integer($1);
  }
  |
  ID {
    if (symtab_lookup($1) == NULL) {
      yyerror("Undeclared variable in expression");
    }
    $$ = ast_id($1);
  }
  |
  STR {
    $$ = ast_string($1);
  }
  |
  TRUE {
    $$ = ast_bool(1);
  }
  |
  FALSE {
    $$ = ast_bool(0);
  }
  |
  expr PLUS expr {
    $$ = ast_operation(PLUS, $1, $3);
  }
  |
  expr MULT expr {
    $$ = ast_operation(MULT, $1, $3);
  }
  |
  expr DIV expr {
    $$ = ast_operation(DIV, $1, $3);
  }
  |
  expr MINUS expr {
    $$ = ast_operation(MINUS, $1, $3);
  }
  |
  expr MOD expr {
    $$ = ast_operation(MOD, $1, $3);
  } 
  |
  expr GT expr {
    $$ = ast_binary(GT, $1, $3);
  }
  |
  expr LT expr {
    $$ = ast_binary(LT, $1, $3);
  }
  |
  expr EQ expr {
    $$ = ast_binary(EQ, $1, $3);
  }
  |
  expr NE expr {
    $$ = ast_binary(NE, $1, $3);
  }
  |
  expr GE expr {
    $$ = ast_binary(GE, $1, $3);
  }
  |
  expr LE expr {
    $$ = ast_binary(LE, $1, $3);
  }
  |
  LPAREN expr RPAREN {
    $$ = $2;
  }
  ;
%%

void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}
