// AST constructor functions

#include <stdlib.h> // for malloc
#include <string.h> // For strdup
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

Cmd *ast_assign(char *var, Expr *expr) {
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = C_ASSIGN;
  node->attr.assign.var = strdup(var); // Important to duplicate the string!
  node->attr.assign.expr = expr;
  return node;
}

Cmd *ast_if(BoolExpr *cond, Cmd *cmd_true, Cmd *cmd_false) {
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->attr.cond.cond = cond;
  node->attr.cond.cmd_true = cmd_true;
  node->attr.cond.cmd_false = cmd_false;

  if (cmd_false == NULL) {
    node->kind = C_IF_THEN;
  } else {
    node->kind = C_IF;
  }
  return node;
}

Cmd *ast_while(BoolExpr *cond, Cmd *cmd) {
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = C_WHILE;
  node->attr.loop.cond = cond;
  node->attr.loop.cmd = cmd;
  return node;
}

Cmd *ast_sequence(Cmd *first, Cmd *second) {
  Cmd *node = (Cmd *)malloc(sizeof(Cmd));
  node->kind = C_SEQUENCE;
  node->attr.sequence.first = first;
  node->attr.sequence.second = second;
  return node;
}

Expr *ast_variable(char *name) {
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_VAR;
  node->attr.name = strdup(name);
  return node;
}
