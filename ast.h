// AST definitions
#ifndef __ast_h__
#define __ast_h__

#include "linkedlist.h"

enum TypeKind { TYPE_INTEGER, TYPE_BOOLEAN };

// AST for expressions
struct _Expr {
  enum {
    E_INTEGER,
    E_ID,         // as
    E_ID_ADDR,    // & x
    E_STRING,     // "teste"
    E_OPERATION,  // a + b
    E_BOOL,       // a < b
    E_BOOL_VALUE, // true
    E_ASSIGNMENT, // b := a
  } kind;

  enum TypeKind type;

  union {
    int value; // for integer values
    char *id;  // for id value

    struct {
      int op; // PLUS, MINUS, etc
      struct _Expr *left;
      struct _Expr *right;
    } op; // for binary expressions

    struct {
      char *id;

      struct _Expr *value;
    } assignment;
  } attr;
};

// AST for commands
struct _Command {

  enum {
    EMPTY_CMD = 0, //;
    EXPR_CMD,      // ola();
    WHILE_CMD,     // while ( )
    IF_CMD,        // if ( )
    IF_ELSE_CMD,   // if ( ) else ( )
    COMPOUND_CMD,  // { }
    PUT_LINE_CMD,
    GET_LINE_CMD
  } command;

  union {
    struct _Expr *value;

    struct {

      struct _Expr *expr;

      struct _Command *cmd;

    } ifCmd, whileCmd;

    struct {

      struct _Expr *expr;

      struct _Command *cmd;

      struct _Command *elsecmd;

    } ifElseCmd;

    struct {

      LinkedList *commands;

    } compound;

    struct {
      struct _Expr *value;
    } putLine;

    struct {
      char *id;
    } getLine;

  } attr;
};

// ------------ Convenience typedefs ----------------------------------------

typedef struct _Expr Expr;

typedef struct _Command Command;

// ------------ Constructor Functions (implementation in ast.c) -------------

Expr *ast_integer(int v);

Expr *ast_id(char *id);

Expr *ast_id_addr(char *id);

Expr *ast_string(char *string);

Expr *ast_bool(int val);

Expr *ast_operation(int op, Expr *left, Expr *right);

Expr *ast_binary(int op, Expr *left, Expr *right);

Expr *ast_assignment(char *id, Expr *value);

Expr *ast_parenthesis(Expr *value);

Command *ast_empty();

Command *ast_if(Expr *expr, Command *cmd);

Command *ast_while(Expr *expr, Command *cmd);

Command *ast_if_then_else(Expr *expr, Command *cmdIf, Command *cmdElse);

Command *ast_expr(Expr *expr);

Command *ast_compound(LinkedList *);

Command *ast_put_line(Expr *value);

Command *ast_get_line(char *id);

#endif
