#include <stdio.h>
#include "printAbsTree.h"
#include "parser.h"

char *oprStr = NULL;
int globalSpaces = 0;

void printSpaces(int spaces) {
  for (int i = 0; i < spaces; i++)
    printf(" ");
}

void printid(char *id) {
  printSpaces(globalSpaces);
  printf("%s\n", id);
}

void printArgs(Expr *expr) {
  printExpr(expr);
  printf(",");
}

char *getid(int op) {

  if (oprStr == NULL) {
    oprStr = malloc(sizeof(char) * 3);
  }

  switch (op) {
  case MINUS:
    oprStr[0] = '-';
    oprStr[1] = '\0';
    break;
  case PLUS:
    oprStr[0] = '+';
    oprStr[1] = '\0';
    break;
  case MULT:
    oprStr[0] = '*';
    oprStr[1] = '\0';
    break;
  case DIV:
    oprStr[0] = '/';
    oprStr[1] = '\0';
    break;
  case MOD:
    oprStr[0] = '%';
    oprStr[1] = '\0';
    break;
  case GT:
    oprStr[0] = '>';
    oprStr[1] = '\0';
    break;
  case LT:
    oprStr[0] = '<';
    oprStr[1] = '\0';
    break;
  case EQ:
    oprStr[0] = '=';
    oprStr[1] = '=';
    oprStr[2] = '\0';
    break;
  case NE:
    oprStr[0] = '!';
    oprStr[1] = '=';
    oprStr[2] = '\0';
    break;
  case GE:
    oprStr[0] = '>';
    oprStr[1] = '=';
    oprStr[2] = '\0';
    break;
  case LE:
    oprStr[0] = '<';
    oprStr[1] = '=';
    oprStr[2] = '\0';
    break;
  default:
    break;
  }

  return oprStr;
}

void printExpr(Expr *expr) { printExprS(expr, globalSpaces); }

void printExprS(Expr *expr, int spaces) {

  switch (expr->kind) {

  case E_INTEGER:
    printSpaces(spaces);
    printf("%d\n", expr->attr.value);
    break;
  case E_ID:
    printSpaces(spaces);
    printf("%s\n", expr->attr.id);
    break;
  case E_ID_ADDR:
    printSpaces(spaces);
    printf("& %s\n", expr->attr.id);
    break;
  case E_STRING:
    printSpaces(spaces);
    printf("\"%s\"\n", expr->attr.id);
    break;
  case E_BOOL_VALUE:
    printSpaces(spaces);
    printf("%s\n", expr->attr.value ? "TRUE" : "FALSE");
    break;
  case E_OPERATION:
  case E_BOOL: {
    printSpaces(spaces);

    printf("%s\n", getid(expr->attr.op.op));

    printExprS(expr->attr.op.left, spaces + 1);
    printExprS(expr->attr.op.right, spaces + 1);
    break;
  }
  case E_ASSIGNMENT:
    printSpaces(spaces);
    printf("%s\n", expr->attr.assignment.id);
    printSpaces(spaces);
    printf("=\n");
    printExprS(expr->attr.assignment.value, spaces + 1);
    break;
  }
}

void printCmdS(Command *cmd, int spaces) {

  switch (cmd->command) {
  case EMPTY_CMD:
    printSpaces(spaces);
    printf("\n");
    break;
  case WHILE_CMD:
    printSpaces(spaces);
    printf("WHILE(\n");
    printExprS(cmd->attr.whileCmd.expr, spaces + 1);
    printSpaces(spaces);
    printf(")\n");
    printCmdS(cmd->attr.whileCmd.cmd, spaces);
    break;
  case IF_CMD:
    printSpaces(spaces);
    printf("IF\n");
    printExprS(cmd->attr.ifCmd.expr, spaces + 1);
    printCmdS(cmd->attr.ifCmd.cmd, spaces);
    break;
  case IF_ELSE_CMD:
    printSpaces(spaces);
    printf("IF\n");
    printExprS(cmd->attr.ifElseCmd.expr, spaces + 1);
    printCmdS(cmd->attr.ifElseCmd.cmd, spaces + 4);
    printSpaces(spaces);
    printf("ELSE\n");
    printCmdS(cmd->attr.ifElseCmd.elsecmd, spaces + 4);
    break;
  case COMPOUND_CMD: {
    int previousSpaces = globalSpaces;
    globalSpaces = spaces;

    iterateList(cmd->attr.compound.commands, (void (*)(void *))printCmd);
    globalSpaces = previousSpaces;
    break;
  }
  case EXPR_CMD:
    printExprS(cmd->attr.value, spaces);
    break;
  case PUT_LINE_CMD:
    printSpaces(spaces);
    printf("OUTPUT: ");
    printExprS(cmd->attr.putLine.value, spaces + 1);
    printSpaces(spaces);
    break;
  case GET_LINE_CMD:
    printSpaces(spaces);
    printf("GET_LINE(%s)\n", cmd->attr.getLine.id);
    break;
  }
}

void printCmd(Command *cmd) { printCmdS(cmd, globalSpaces); }
