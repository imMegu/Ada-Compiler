#ifndef TAC_H
#define TAC_H

#include "ast.h"
typedef enum {
  TAC_ADD,
  TAC_SUB,
  TAC_MUL,
  TAC_DIV,
  TAC_MOD, // Aritmética
  TAC_EQ,
  TAC_NE,
  TAC_LT,
  TAC_LE,
  TAC_GT,
  TAC_GE,        // Comparação
  TAC_MOV,       // Atribuição (copy)
  TAC_LABEL,     // Label de salto
  TAC_JMP,       // Salto incondicional (goto)
  TAC_IFZ,       // Salto condicional (se zero)
  TAC_PRINT_INT, // Print integer
  TAC_PRINT_STR, // Print string
  TAC_READ,      // Read integer
  TAC_RET,       // Return
  TAC_AND,
  TAC_OR,
  TAC_XOR,
  TAC_NOT,
  TAC_AND_THEN,
  TAC_OR_ELSE
} TACOp;

// Estrutura de uma instrução TAC
typedef struct TAC {
  TACOp op;
  char *result;
  char *arg1;
  char *arg2;
  struct TAC *next;
  struct TAC *prev;
} TAC;

TAC *tac_create(TACOp op, char *result, char *arg1, char *arg2);
TAC *tac_join(TAC *l1, TAC *l2);
TAC *generate_code(Command *root);

void tac_dump(TAC *tac);

#endif
