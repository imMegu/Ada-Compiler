#include <stdio.h>
#include <string.h>
#include "parser.h"

// --- Simple Global Environment (Symbol Table) ---

// Max number of variables the simple environment can hold
#define MAX_VARS 100

typedef struct {
  char *name; // Variable name
  int value;  // Variable value
  int used;   // Flag if the slot is in use (for simplicity)
} EnvEntry;

EnvEntry environment[MAX_VARS];
int env_count = 0;

// Function to find a variable's index in the environment
int find_var(char *name) {
  for (int i = 0; i < env_count; i++) {
    if (environment[i].used && strcmp(environment[i].name, name) == 0) {
      return i;
    }
  }
  return -1; // Not found
}

// Function to set a variable's value. Creates it if it doesn't exist.
void set_var(char *name, int value) {
  int index = find_var(name);

  if (index != -1) {
    // Variable exists, update value
    environment[index].value = value;
  } else if (env_count < MAX_VARS) {
    // Variable doesn't exist, create new entry
    environment[env_count].name = strdup(name); // Duplicate string
    environment[env_count].value = value;
    environment[env_count].used = 1;
    env_count++;
  } else {
    yyerror("Environment is full! Cannot create new variable.");
  }
}

// Function to get a variable's value.
int get_var(char *name) {
  int index = find_var(name);
  if (index != -1) {
    return environment[index].value;
  }
  // Error: Variable not initialized or declared
  char msg[256];
  sprintf(msg, "Undefined variable: %s", name);
  yyerror(msg);
  return 0; // Return 0 as a default/error value
}

// ------------------- Evaluation Functions ----------------------

int eval(Expr *expr) {
  int result = 0;
  if (expr == 0) {
    yyerror("Null expression!!");
  } else if (expr->kind == E_INTEGER) {
    result = expr->attr.value;
  } else if (expr->kind == E_VAR) {
    result = get_var(expr->attr.name);
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

// ------------- Command Execution Function ----------------

int execCmd(Cmd *cmd) {
  if (cmd == NULL) {
    // A NULL command is an empty command, which is a no-op.
    return 0;
  }

  switch (cmd->kind) {

  case C_ASSIGN: {
    // var = expr
    char *varName = cmd->attr.assign.var;
    int value = eval(cmd->attr.assign.expr);
    set_var(varName, value);
    printf("Executed: %s = %d\n", varName, value);
    break;
  }

  case C_SEQUENCE:
    // C_SEQUENCE: first ; second
    execCmd(cmd->attr.sequence.first);
    execCmd(cmd->attr.sequence.second);
    break;

  case C_IF_THEN: {
    // C_IF_THEN: if cond then cmd_true
    if (evalBool(cmd->attr.cond.cond)) {
      execCmd(cmd->attr.cond.cmd_true);
    }
    break;
  }

  case C_IF: {
    // C_IF: if cond then cmd_true else cmd_false
    if (evalBool(cmd->attr.cond.cond)) {
      execCmd(cmd->attr.cond.cmd_true);
    } else {
      execCmd(cmd->attr.cond.cmd_false);
    }
    break;
  }

  case C_WHILE:
    // C_WHILE: while cond do cmd
    while (evalBool(cmd->attr.loop.cond)) {
      execCmd(cmd->attr.loop.cmd);
    }
    break;
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

  // Cleanup the environment (free variable names)
  for (int i = 0; i < env_count; i++) {
    if (environment[i].used) {
      free(environment[i].name);
    }
  }

  return 0;
}
