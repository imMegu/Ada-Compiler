%{
// HEADERS
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

"while" { return WHILE; }
"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"="     { return ASSIGN; }
";"     { return SEMICOLON; }
"{"     { return LBRACE; }
"}"     { return RBRACE; }
"("     { return LPAREN; }
")"     { return RPAREN; }
[a-zA-Z]+ { yylval.stringValue = strdup(yytext); return ID; }

\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return TIMES; }
"/" { return DIV; }
"%" { return MOD; }
"==" { return EQ; }
"!=" { return NE; }
"<"  { return LT; }
">"  { return GT; }
"<=" { return LE; }
">=" { return GE; }
.  { yyerror("unexpected character"); }
%%
