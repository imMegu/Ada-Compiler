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

\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}

"+"   { return PLUS; }
"-"   { return MINUS; }
"*"   { return MULT; }
"/"   { return DIV; }
"rem" { return MOD; }

"=" { return EQ; }
"/=" { return NE; }
"<"  { return LT; }
">"  { return GT; }
"<=" { return LE; }
">=" { return GE; }

"while" { return WHILE; }
"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"elsif" { return ELSIF; }
"loop"  { return LOOP; }
"end"   { return END; }
"and"      { return AND; }
"or"       { return OR; }
"and then" { return AND_THEN; }
"or else"  { return OR_ELSE; }
"not"      { return NOT; }
"xor"      { return XOR; }

"procedure" { return PROCEDURE; }
"begin"     { return BEG;}
"Main"      { return MAIN; }
"is"        { return IS; }

":="     { return ASSIGN; }
":"         { return COLON; }
";"     { return SEMICOLON; }
","     { return COMMA; }
"("     { return LPAREN; }
")"     { return RPAREN; }

"true" { return TRUE; }
"false" { return FALSE; }
"Integer" { return INT_TYPE; }
"Boolean" { return BOOL_TYPE; }

"Get_Line" { return GET_LINE; }
"Put_Line" { return PUT_LINE; }

\".*\" {
  yylval.strValue = strdup(yytext);
  return STR;
}

[a-zA-Z][0-9a-zA-Z\_]* {
  yylval.idValue = strdup(yytext); 
  return ID;
}

.  { yyerror("unexpected character"); }
%%
