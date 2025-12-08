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
  ELSIF  // ELSIF
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
%type <cmdValue> if_tail
%type <cmdList> cmd_list
%type <idList> id_list
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
extern void semantic_error(const char* msg);

extern Command *root;
}

%%
program: 
  PROCEDURE MAIN IS { symtab_enter_scope(); } decls BEG cmd_list END MAIN SEMICOLON {
    root = ast_compound($7); // $7 is cmd_list now
    symtab_exit_scope(); // Exit global scope after parsing
  }

decls:
  /* empty */ {
    $$ = mkEmptyList();
  }
  |
  decls id_list COLON type SEMICOLON {
    // Iterate over the list of IDs and insert each into the symbol table
    LinkedList *ids = $2;
    Node *current = ids->first;
    while (current != NULL) {
      char *var_name = (char *)current->value;
      if (symtab_insert(var_name, $4) != 0) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "Variable '%s' already declared in this scope", var_name);
        semantic_error(error_msg);
        exit(1);
      }
      current = current->next;
    }
    current = ids->first;
    while (current != NULL) {
        free(current->value); // Liberta o strdup() do scanner
        current = current->next;
    }
    
    $$ = concatLists($1, $2);
  } 
  ;

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
  // Uma lista de comandos pode estar vazia 
  {
    $$ = mkEmptyList();
  }
  |
  //Ou pode ser uma lista, seguida por mais um comando 
  cmd_list cmd {
    $$ = concatLast($1, $2);
  }
  ;

cmd:
  ID ASSIGN expr SEMICOLON {
    if (symtab_lookup($1) == NULL) {
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg), "Undeclared variable '%s' in assignment", $1);
      semantic_error(error_msg);
      exit(1);
    }

    enum TypeKind var_type = symtab_lookup($1)->type; 
    enum TypeKind expr_type = $3->type;
    
    if (var_type != expr_type) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                 "Type Mismatch: Cannot assign expression of type %s to variable '%s' of type %s.", 
                 (expr_type == TYPE_INTEGER ? "Integer" : "Boolean"),
                 $1,
                 (var_type == TYPE_INTEGER ? "Integer" : "Boolean"));
        semantic_error(error_msg);
        exit(1);
    }

    $$ = ast_expr(ast_assignment($1, $3));
    free($1);
  }
  |
  IF expr THEN cmd_list if_tail SEMICOLON { 
    if ($2->type != TYPE_BOOLEAN) {
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg), 
               "Type Mismatch: IF condition must be Boolean, got %s",
               ($2->type == TYPE_INTEGER ? "Integer" : "Unknown"));
      semantic_error(error_msg);
      exit(1);
    }

    Command *if_body = ast_compound($4);
    Command *else_block = $5; // $5 é o resultado de if_tail

    if (else_block == NULL) {
      // Não há else nem elsif
      $$ = ast_if($2, if_body); 
    } else {
      // Há um else ou um elsif (que é tratado como um else)
      $$ = ast_if_then_else($2, if_body, else_block);
    }
  }
  |
  WHILE expr LOOP cmd_list END LOOP SEMICOLON {
    Command *body = ast_compound($4);
    $$ = ast_while($2, body);
  }
  |
  PUT_LINE LPAREN expr RPAREN SEMICOLON {    
    $$ = ast_put_line($3);
  }
  |
  GET_LINE LPAREN ID RPAREN SEMICOLON {
    Symbol* s = symtab_lookup($3); 
    if (s == NULL) { // Check if ID is declared
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg), "Undeclared variable '%s' in Get_Line", $3);
      semantic_error(error_msg);
      exit(1);
    }
    if (s->type != TYPE_INTEGER) { 
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg), "Type Mismatch: Get_Line requires an Integer variable, got %s for '%s'", 
                (s->type == TYPE_BOOLEAN ? "Boolean" : "Unknown"), $3);
      semantic_error(error_msg);
      exit(1);
    }
    $$ = ast_get_line($3);
    free($3);
  }

if_tail:
  END IF {
    $$ = NULL; // Fim da cadeia, sem else
  }
  |
  ELSE cmd_list END IF {
    $$ = ast_compound($2); // Bloco ELSE final
  }
  |
  ELSIF expr THEN cmd_list if_tail {
    if ($2->type != TYPE_BOOLEAN) {
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg), 
               "Type Mismatch: ELSIF condition must be Boolean, got %s",
               ($2->type == TYPE_INTEGER ? "Integer" : "Unknown"));
      semantic_error(error_msg);
      exit(1);
    }

    Command *elsif_body = ast_compound($4);
    Command *next_tail = $5; 
    Command *inner_if;

    if (next_tail == NULL) {
      // Este ELSIF não é seguido por mais nada
      inner_if = ast_if($2, elsif_body);
    } else {
      // Este ELSIF é seguido por outro elsif/else
      inner_if = ast_if_then_else($2, elsif_body, next_tail);
    }
    
    // Devolvemos o 'inner_if' como se fosse um bloco ELSE
    $$ = inner_if;
  }
;

expr:
  INT {
    $$ = ast_integer($1);
    $$->type = TYPE_INTEGER;
  }
  |
  ID {
    Symbol *symbol = symtab_lookup($1);
    if (symbol == NULL) {
      char error_msg[256];
      snprintf(error_msg, sizeof(error_msg), "Undeclared variable '%s' in expression", $1);
      semantic_error(error_msg);
      exit(1);
    }
    $$ = ast_id($1);
    $$->type = symbol->type; 
    free($1);
  }
  |  
  STR {
    $$ = ast_string($1);
    $$->type = TYPE_STRING;
  }
  |
  TRUE {
    $$ = ast_bool(1);
    $$->type = TYPE_BOOLEAN;
  }
  |
  FALSE {
    $$ = ast_bool(0);
    $$->type = TYPE_BOOLEAN;
  }
  |
  expr PLUS expr {
    if ($1->type != TYPE_INTEGER || $3->type != TYPE_INTEGER) {
        semantic_error("Type Mismatch: '+' operation requires Integer operands.");
        exit(1);
    }
    $$ = ast_operation(PLUS, $1, $3);
    $$->type = TYPE_INTEGER;
  }
  |
  expr MULT expr {
    if ($1->type != TYPE_INTEGER || $3->type != TYPE_INTEGER) {
        semantic_error("Type Mismatch: '*' operation requires Integer operands.");
        exit(1);
    }
    $$ = ast_operation(MULT, $1, $3);
    $$->type = TYPE_INTEGER;
  }
  |
  expr DIV expr {
    if ($1->type != TYPE_INTEGER || $3->type != TYPE_INTEGER) {
        semantic_error("Type Mismatch: '/' operation requires Integer operands.");
        exit(1);
    }
    $$ = ast_operation(DIV, $1, $3);
    $$->type = TYPE_INTEGER;
  }
  |
  expr MINUS expr {
    if ($1->type != TYPE_INTEGER || $3->type != TYPE_INTEGER) {
        semantic_error("Type Mismatch: '-' operation requires Integer operands.");
        exit(1);
    }
    $$ = ast_operation(MINUS, $1, $3);
    $$->type = TYPE_INTEGER;
  }
  |
  expr MOD expr {
    if ($1->type != TYPE_INTEGER || $3->type != TYPE_INTEGER) {
        semantic_error("Type Mismatch: 'rem' operation requires Integer operands.");
        exit(1);
    }
    $$ = ast_operation(MOD, $1, $3);
    $$->type = TYPE_INTEGER;
  } 
  |
expr GT expr {
    if ($1->type != $3->type) {
        semantic_error("Type Mismatch: Cannot compare operands of different types for '>' operation.");
        exit(1);
    }
    if ($1->type == TYPE_BOOLEAN) {
        semantic_error("Type Mismatch: Invalid operation '>' for Boolean type.");
        exit(1);
    }
    $$ = ast_binary(GT, $1, $3);
    $$->type = TYPE_BOOLEAN;
  }
  |
  expr LT expr {
    if ($1->type != $3->type) {
        semantic_error("Type Mismatch: Cannot compare operands of different types for '<' operation.");
        exit(1);
    }
    if ($1->type == TYPE_BOOLEAN) {
        semantic_error("Type Mismatch: Invalid operation '<' for Boolean type.");
        exit(1);
    }
    $$ = ast_binary(LT, $1, $3);
    $$->type = TYPE_BOOLEAN;
  }
  |
  expr EQ expr {
    if ($1->type != $3->type) {
        semantic_error("Type Mismatch: Cannot compare operands of different types for '=' operation.");
        exit(1);
    }
    $$ = ast_binary(EQ, $1, $3);
    $$->type = TYPE_BOOLEAN;
  }
  |
expr NE expr {
    if ($1->type != $3->type) {
        semantic_error("Type Mismatch: Cannot compare operands of different types for '/=' operation.");
        exit(1);
    }
    $$ = ast_binary(NE, $1, $3);
    $$->type = TYPE_BOOLEAN;
  }
  |
  expr GE expr {
    if ($1->type != $3->type) {
        semantic_error("Type Mismatch: Cannot compare operands of different types for '>=' operation.");
        exit(1);
    }
    if ($1->type == TYPE_BOOLEAN) {
        semantic_error("Type Mismatch: Invalid operation '>=' for Boolean type.");
        exit(1);
    }
    $$ = ast_binary(GE, $1, $3);
    $$->type = TYPE_BOOLEAN;
  }
  |
  expr LE expr {
    if ($1->type != $3->type) {
        semantic_error("Type Mismatch: Cannot compare operands of different types for '<=' operation.");
        exit(1);
    }
    if ($1->type == TYPE_BOOLEAN) {
        semantic_error("Type Mismatch: Invalid operation '<=' for Boolean type.");
        exit(1);
    }
    $$ = ast_binary(LE, $1, $3);
    $$->type = TYPE_BOOLEAN;
  }
  |
  LPAREN expr RPAREN {
    $$ = $2;
  }
  ;
%%

void yyerror(const char* err) {
  yynerrs++;
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}

void semantic_error(const char* err) {
  printf("Line %d: %s\n", yyline, err);
}
