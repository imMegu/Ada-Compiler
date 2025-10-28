#include <stdio.h>
#include "parser.h"

int eval(Expr *expr) {
  int result = 0;
  if (expr == 0) {
    yyerror("Null expression!!");
  } else if (expr->kind == E_INTEGER) {
    result = expr->attr.value;
  } else if (expr->kind == E_OPERATION) {
    int vLeft = eval(expr->attr.op.left);
    int vRight = eval(expr->attr.op.right);
    switch (expr->attr.op.op) {
    case PLUS:
      result = vLeft + vRight;
      break;
    case MINUS:
      result = vLeft - vRight;
      break;
    case TIMES:
      result = vLeft * vRight;
      break;
    case DIV:
      result = vLeft / vRight;
      break;
    case MOD:
      result = vLeft % vRight;
      break;

    default:
      yyerror("Unknown op!");
    }
  }
  return result;
}

int evalBool(BoolExpr *expr) {
  if (expr == 0) {
    yyerror("Null boolean expression!!");
    return 0;
  }

  if (expr->kind == B_INTEGER) {
    return expr->attr.value != 0 ? 1 : 0;
  } else if (expr->kind == B_RELATION) {
    // Evaluate the arithmetic children using the existing eval() function
    int vLeft = eval(expr->attr.rel.left);
    int vRight = eval(expr->attr.rel.right);

    // Perform the relational operation
    switch (expr->attr.rel.op) {
    case EQ: // ==
      return vLeft == vRight;
    case NE: // !=
      return vLeft != vRight;
    case LT: // <
      return vLeft < vRight;
    case GT: // >
      return vLeft > vRight;
    case LE: // <=
      return vLeft <= vRight;
    case GE: // >=
      return vLeft >= vRight;

    default:
      yyerror("Unknown relational operator!");
      return 0;
    }
  }
  return 0;
}

int main(int argc, char **argv) {

  --argc;
  ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  yyparse();
  return 0;
}
