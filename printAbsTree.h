#ifndef PRINTABSTREE_H
#define PRINTABSTREE_H

#include "ast.h"

void printExpr(Expr *);

void printExprS(Expr *, int);

void printCmd(Command *);

void printCmdS(Command *, int);

#endif /* PRINTABSTREE_H */
