// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum { E_INTEGER, E_OPERATION, E_VAR } kind;
  union {
    int value; // for E_INTEGER
    struct {
      int op; // PLUS, MINUS, etc
      struct _Expr *left;
      struct _Expr *right;
    } op;       // for E_OPERATION
    char *name; // for E_VAR (variable name)
  } attr;
};

// AST for boolean expressions
struct _BoolExpr {
  enum {
    B_INTEGER, // For simple boolean values (0 or 1)
    B_RELATION // For relational operations (==, !=, <, etc.)
  } kind;
  union {
    int value; // for boolean integer values (0 or 1)
    struct {
      int op; // EQ, NE, LT, GT, LE, GE (New tokens to be defined later)
      struct _Expr *left;
      struct _Expr *right;
    } rel; // for relational expressions
  } attr;
};

// AST for commands
struct _Cmd {
  enum {
    C_ASSIGN,   // var = expr
    C_IF,       // if (boolExpr) cmd1 else cmd2
    C_WHILE,    // while (boolExpr) cmd
    C_SEQUENCE, // cmd1 ; cmd2
    C_IF_THEN,  // if (boolExpr) cmd
  } kind;
  union {
    struct {
      char *var; // For simplicity, assume variable names are single tokens
      struct _Expr *expr;
    } assign;
    struct {
      struct _BoolExpr *cond;
      struct _Cmd *cmd_true;
      struct _Cmd *cmd_false; // NULL for C_IF_THEN
    } cond;
    struct {
      struct _BoolExpr *cond;
      struct _Cmd *cmd;
    } loop;
    struct {
      struct _Cmd *first;
      struct _Cmd *second;
    } sequence;
  } attr;
};

// ------------ Convenience typedefs --------------------------

typedef struct _Expr Expr;
typedef struct _BoolExpr BoolExpr;
typedef struct _Cmd Cmd;

// ------------ Constructor function declarations -------------

// For Expressions
Expr *ast_integer(int v);
Expr *ast_operation(int op, Expr *left, Expr *right);

// For Boolean Expressions
BoolExpr *ast_boolean_integer(int v);
BoolExpr *ast_relation(int op, Expr *left, Expr *right);

// For Commands
Cmd *ast_assign(char *var, Expr *expr);
Cmd *ast_if(BoolExpr *cond, Cmd *cmd_true,
            Cmd *cmd_false); // Used for both if-then and if-then-else
Cmd *ast_while(BoolExpr *cond, Cmd *cmd);
Cmd *ast_sequence(Cmd *first, Cmd *second);
Expr *ast_variable(char *name);

#endif
