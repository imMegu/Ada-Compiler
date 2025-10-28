%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

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
