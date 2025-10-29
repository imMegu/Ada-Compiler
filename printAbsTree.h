#ifndef PRINTABSTREE_H
#define PRINTABSTREE_H

#include "ast.h"

void printExpr(BoolExpr *expr);

// Example output: ((3 + 5) * (10 - 2))
void printExpr(Expr *expr);

/*
 * Example output:
 *   OP: TIMES
 *     OP: PLUS
 *       INT: 3
 *       INT: 5
 *     OP: MINUS
 *       INT: 10
 *       INT: 2
 */

void printCmd(Cmd *cmd);

void printExprTree(Expr *expr, int depth);

#endif /* PRINTABSTREE_H */
