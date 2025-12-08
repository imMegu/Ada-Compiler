/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 63 "parser.bison"

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "linkedlist.h"
#include "symboltable.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
extern void semantic_error(const char* msg);

extern Command *root;

#line 66 "parser.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    STR = 259,                     /* STR  */
    PLUS = 260,                    /* PLUS  */
    MINUS = 261,                   /* MINUS  */
    MULT = 262,                    /* MULT  */
    DIV = 263,                     /* DIV  */
    MOD = 264,                     /* MOD  */
    EQ = 265,                      /* EQ  */
    NE = 266,                      /* NE  */
    LT = 267,                      /* LT  */
    GT = 268,                      /* GT  */
    LE = 269,                      /* LE  */
    GE = 270,                      /* GE  */
    TRUE = 271,                    /* TRUE  */
    FALSE = 272,                   /* FALSE  */
    INT_TYPE = 273,                /* INT_TYPE  */
    BOOL_TYPE = 274,               /* BOOL_TYPE  */
    WHILE = 275,                   /* WHILE  */
    IF = 276,                      /* IF  */
    THEN = 277,                    /* THEN  */
    ELSE = 278,                    /* ELSE  */
    ELSIF = 279,                   /* ELSIF  */
    LOOP = 280,                    /* LOOP  */
    END = 281,                     /* END  */
    ASSIGN = 282,                  /* ASSIGN  */
    SEMICOLON = 283,               /* SEMICOLON  */
    LPAREN = 284,                  /* LPAREN  */
    RPAREN = 285,                  /* RPAREN  */
    ID = 286,                      /* ID  */
    PROCEDURE = 287,               /* PROCEDURE  */
    IS = 288,                      /* IS  */
    MAIN = 289,                    /* MAIN  */
    COLON = 290,                   /* COLON  */
    COMMA = 291,                   /* COMMA  */
    BEG = 292,                     /* BEG  */
    GET_LINE = 293,                /* GET_LINE  */
    PUT_LINE = 294                 /* PUT_LINE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 40 "parser.bison"

  int intValue;
  int typeKind;
  char *idValue, *strValue;
  Expr* exprValue;
  Command* cmdValue;
  LinkedList* cmdList;
  LinkedList* idList;

#line 132 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
