
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum { E_INTEGER, E_OPERATION } kind;
  union {
    int value; // for integer values
    struct {
      int op; // PLUS, MINUS, etc
      struct _Expr *left;
      struct _Expr *right;
    } op; // for binary expressions
  } attr;
};

typedef struct _Expr Expr; // Convenience typedef

// Constructor functions (see implementation in ast.c)
Expr *ast_integer(int v);
Expr *ast_operation(int op, Expr *left, Expr *right);

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

typedef struct _BoolExpr BoolExpr; // Convenience typedef

// Constructor functions for boolean expressions
BoolExpr *ast_boolean_integer(int v);
BoolExpr *ast_relation(int op, Expr *left, Expr *right);

#endif
