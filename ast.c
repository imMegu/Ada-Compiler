// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h"    // AST header

Expr *ast_integer(int v) {
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr *ast_operation(int op, Expr *left, Expr *right) {
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.op = op;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

BoolExpr *ast_boolean_integer(int v) {
  // We use BoolExpr* for the return type
  BoolExpr *node = (BoolExpr *)malloc(sizeof(BoolExpr));
  node->kind = B_INTEGER;
  node->attr.value = v;
  return node;
}

BoolExpr *ast_relation(int op, Expr *left, Expr *right) {
  // We use BoolExpr* for the return type
  BoolExpr *node = (BoolExpr *)malloc(sizeof(BoolExpr));
  node->kind = B_RELATION;
  node->attr.rel.op = op;
  node->attr.rel.left = left;
  node->attr.rel.right = right;
  return node;
}
