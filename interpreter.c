#include <stdio.h>
#include "parser.h"
#include "printAbsTree.h"
#include "symboltable.h"
extern int yynerrs;

Command *root = NULL;

int var_values_integer[1024]; // Simple array for integer values
char *var_names[1024];        // Corresponding names
int var_count = 0;

int get_var_index(char *id) {
  for (int i = 0; i < var_count; i++) {
    if (strcmp(var_names[i], id) == 0) {
      return i;
    }
  }
  return -1; // Not found
}

void set_var_value(char *id, int value) {
  int index = get_var_index(id);
  if (index == -1) {
    // Simple storage for new variables found at runtime
    if (var_count < 1024) {
      var_names[var_count] = strdup(id);
      var_values_integer[var_count] = value;
      var_count++;
    } else {
      yyerror("Out of variable storage space!");
    }
  } else {
    var_values_integer[index] = value;
  }
}

int get_var_value(char *id) {
  int index = get_var_index(id);
  if (index != -1) {
    return var_values_integer[index];
  }
  // Should not happen if parser performs checks
  yyerror("Runtime: Undeclared variable accessed!");
  return 0;
}

int eval(Expr *expr) {
  int result = 0;
  if (expr == 0) {
    yyerror("Null expression!!");
  } else if (expr->kind == E_INTEGER) {
    result = expr->attr.value;
  } else if (expr->kind == E_BOOL_VALUE) {
    result = expr->attr.value;
  } else if (expr->kind == E_OPERATION) {
    int vLeft = eval(expr->attr.op.left);
    int vRight = eval(expr->attr.op.right);

    switch (expr->attr.op.op) {
    case PLUS:
      result = vLeft + vRight;
      break;
    case MULT:
      result = vLeft * vRight;
      break;
    case DIV:
      if (vRight == 0) {
        yyerror("Runtime Error: Division by zero!");
        exit(1);
      }
      result = vLeft / vRight;
      break;
    case MINUS:
      result = vLeft - vRight;
      break;
    case MOD:
      result = vLeft % vRight;
      break;
    case GT:
      result = vLeft > vRight;
      break;
    case LT:
      result = vLeft < vRight;
      break;
    case EQ:
      result = vLeft == vRight;
      break;
    case NE:
      result = vLeft != vRight;
      break;
    case GE:
      result = vLeft >= vRight;
      break;
    case LE:
      result = vLeft <= vRight;
      break;
    default:
      yyerror("Unknown op!");
    }
  } else if (expr->kind == E_ID) {
    return get_var_value(expr->attr.id); // Get value for variable
  } else if (expr->kind == E_ASSIGNMENT) {
    int val = eval(expr->attr.assignment.value);
    set_var_value(expr->attr.assignment.id, val);
    return val;
  }
  return result;
}

void execCmd(Command *cmd) {
  if (cmd == NULL)
    return;

  switch (cmd->command) {
  case EXPR_CMD: // For ID ASSIGN expr SEMICOLON
    eval(cmd->attr.value);
    break;

  case COMPOUND_CMD:
    Node *current = cmd->attr.compound.commands->first;
    while (current != NULL) {
      execCmd((Command *)current->value);
      current = current->next;
    }
    break;

  case IF_CMD:
    if (eval(cmd->attr.ifCmd.expr)) {
      execCmd(cmd->attr.ifCmd.cmd);
    }
    break;

  case IF_ELSE_CMD:
    if (eval(cmd->attr.ifElseCmd.expr)) {
      execCmd(cmd->attr.ifElseCmd.cmd);
    } else {
      execCmd(cmd->attr.ifElseCmd.elsecmd);
    }
    break;

  case WHILE_CMD:
    while (eval(cmd->attr.whileCmd.expr)) {
      execCmd(cmd->attr.whileCmd.cmd);
    }
    break;

  case PUT_LINE_CMD:
    printf("OUTPUT: ");
    if (cmd->attr.putLine.value->kind == E_STRING) {
      printf("%s\n", cmd->attr.putLine.value->attr.id);
    } else {
      printf("%d\n", eval(cmd->attr.putLine.value));
    }
    break;

  case GET_LINE_CMD:
    printf("INPUT {%s}: ", cmd->attr.getLine.id);
    int input_val;
    if (scanf("%d", &input_val) == 1) {
      set_var_value(cmd->attr.getLine.id, input_val);
    } else {
      yyerror("Invalid input for Get_Line");
    }
    break;

  default:
    break;
  }
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
  } 
  yyparse(); 

  if (yynerrs == 0) {
    printf("--- Running interpreter ---\n");
    execCmd(root);
  } else {
    printf("--- Compilation failed with %d errors. Execution halted. ---\n", yynerrs);
  }

  return 0;
}
