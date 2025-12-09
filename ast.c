// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h"    // AST header
#include <string.h>

Expr *ast_integer(int v) {
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->type = TYPE_INTEGER;
  node->attr.value = v;
  return node;
}

Expr *ast_id(char *id) {
  Expr *node = (Expr *)malloc(sizeof(Expr));

  node->kind = E_ID;
  node->attr.id = strdup(id);

  return node;
}

Expr *ast_id_addr(char *id) {
  Expr *node = (Expr *)malloc(sizeof(Expr));

  node->kind = E_ID_ADDR;
  node->attr.id = id;

  return node;
}

Expr *ast_string(char *string) {
  Expr *node = (Expr *)malloc(sizeof(Expr));

  node->kind = E_STRING;
  node->type = TYPE_STRING; 

  int strLen = strlen(string);
  int newLen = strLen - 2; // O comprimento da string sem aspas

  char *finalid = malloc(sizeof(char) * (newLen + 1));

  memcpy(finalid, (string + 1), newLen);

  finalid[newLen] = '\0';

  node->attr.id = finalid;

  free(string); 

  return node;
}

Expr *ast_bool(int value) {
  Expr *expr = malloc(sizeof(Expr));

  expr->kind = E_BOOL_VALUE;
  expr->type = TYPE_BOOLEAN;
  expr->attr.value = value;

  return expr;
}

Expr *ast_operation(int op, Expr *left, Expr *right) {
  Expr *node = (Expr *)malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.op = op;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}

Expr *ast_binary(int op, Expr *left, Expr *right) {

  Expr *node = (Expr *)malloc(sizeof(Expr));

  node->kind = E_OPERATION;
  node->attr.op.op = op;
  node->attr.op.left = left;
  node->attr.op.right = right;

  return node;
}

Expr *ast_assignment(char *id, Expr *value) {
  Expr *node = (Expr *)malloc(sizeof(Expr));

  node->kind = E_ASSIGNMENT;
  node->attr.assignment.id = strdup(id);
  node->attr.assignment.value = value;

  return node;
}

Command *ast_empty() {
  Command *cmd = malloc(sizeof(Command));

  cmd->command = EMPTY_CMD;

  return cmd;
}

Command *ast_if(Expr *expr, Command *cmd) {

  Command *nCmd = malloc(sizeof(Command));

  nCmd->command = IF_CMD;

  nCmd->attr.ifCmd.expr = expr;
  nCmd->attr.ifCmd.cmd = cmd;

  return nCmd;
}

Command *ast_while(Expr *expr, Command *cmd) {
  Command *nCmd = malloc(sizeof(Command));

  nCmd->command = WHILE_CMD;

  nCmd->attr.whileCmd.expr = expr;
  nCmd->attr.whileCmd.cmd = cmd;

  return nCmd;
}

Command *ast_if_then_else(Expr *expr, Command *ifThen, Command *elseThen) {

  Command *nCmd = malloc(sizeof(Command));

  nCmd->command = IF_ELSE_CMD;

  nCmd->attr.ifElseCmd.expr = expr;
  nCmd->attr.ifElseCmd.cmd = ifThen;
  nCmd->attr.ifElseCmd.elsecmd = elseThen;

  return nCmd;
}

Command *ast_expr(Expr *expr) {
  Command *nCmd = malloc(sizeof(Command));

  nCmd->command = EXPR_CMD;

  nCmd->attr.value = expr;

  return nCmd;
}

Command *ast_compound(LinkedList *list) {
  Command *nCmd = malloc(sizeof(Command));

  nCmd->command = COMPOUND_CMD;

  nCmd->attr.compound.commands = list;

  return nCmd;
}

Command *ast_put_line(Expr *expr) {
  Command *nCmd = malloc(sizeof(Command));
  nCmd->command = PUT_LINE_CMD;
  nCmd->attr.putLine.value = expr;
  return nCmd;
}

Command *ast_get_line(char *id) {
  Command *nCmd = malloc(sizeof(Command));
  
  if (!nCmd) exit(EXIT_FAILURE); // Boa prática: verificar malloc
  
  nCmd->command = GET_LINE_CMD;
  
  // CORREÇÃO CRÍTICA: Faz a cópia profunda do nome da variável
  nCmd->attr.getLine.id = strdup(id);
  
  if (!nCmd->attr.getLine.id) {
      // Se strdup falhar, liberta o nó e sai
      free(nCmd);
      exit(EXIT_FAILURE);
  }
  
  return nCmd;
}
