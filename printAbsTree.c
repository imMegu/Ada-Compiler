#include <stdio.h>
#include "ast.h"
#include "parser.h"

void printBool(BoolExpr *exp) {
  if (exp == NULL) {
    printf("NULL");
    return;
  }

  if (exp->kind == B_INTEGER) {
    printf("BOOL_INT(%d)", exp->attr.value);
  } else if (exp->kind == B_RELATION) {
    // Relational expressions are enclosed in brackets []
    printf("[");
    printExpr(
        exp->attr.rel.left); // printExpr handles the arithmetic sub-expression

    switch (exp->attr.rel.op) {
    case EQ:
      printf(" == ");
      break;
    case NE:
      printf(" != ");
      break;
    case LT:
      printf(" < ");
      break;
    case GT:
      printf(" > ");
      break;
    case LE:
      printf(" <= ");
      break;
    case GE:
      printf(" >= ");
      break;
    default:
      printf(" ? ");
      break;
    }

    printExpr(
        exp->attr.rel.right); // printExpr handles the other sub-expression
    printf("]");
  }
}

void printExpr(Expr *expr) {
  if (expr == NULL) {
    printf("NULL");
    return;
  }

  if (expr->kind == E_INTEGER) {
    printf("%d", expr->attr.value);
  } else if (expr->kind == E_OPERATION) {
    printf("(");
    printExpr(expr->attr.op.left);

    switch (expr->attr.op.op) {
    case PLUS:
      printf(" + ");
      break;
    case MINUS:
      printf(" - ");
      break;
    case TIMES:
      printf(" * ");
      break;
    case DIV:
      printf(" / ");
      break;
    case MOD:
      printf(" %% ");
      break;
    default:
      printf(" ? ");
      break;
    }

    printExpr(expr->attr.op.right);
    printf(")");
  }
}

// Alternative: Print in tree format with indentation
void printExprTree(Expr *expr, int depth) {
  if (expr == NULL) {
    return;
  }

  for (int i = 0; i < depth; i++) {
    printf("  ");
  }

  if (expr->kind == E_INTEGER) {
    printf("INT: %d\n", expr->attr.value);
  } else if (expr->kind == E_OPERATION) {
    const char *opName;
    switch (expr->attr.op.op) {
    case PLUS:
      opName = "PLUS";
      break;
    case MINUS:
      opName = "MINUS";
      break;
    case TIMES:
      opName = "TIMES";
      break;
    case DIV:
      opName = "DIV";
      break;
    case MOD:
      opName = "MOD";
      break;
    default:
      opName = "UNKNOWN";
      break;
    }
    printf("OP: %s\n", opName);
    printExprTree(expr->attr.op.left, depth + 1);
    printExprTree(expr->attr.op.right, depth + 1);
  }
}
