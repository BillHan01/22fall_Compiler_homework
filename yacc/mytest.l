/*mytest.l*/
%{
#include <stdio.h>
#include "y.tab.h"
void yyerror(char *);
%}

%%


"+"							return ADD;
"-"							return SUB;
"*"							return MUL;
"/"							return DIV;
"i"                     	return ID;
"("                         return LB;
")"                         return RB;
\n                          return CR;
[ \t]+                     /* ignore whitespace */;
.
%%
