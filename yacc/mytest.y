/*mytest.y*/
%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
%}

%token ADD SUB MUL DIV ID CR LB RB 
%left RB
%left ADD SUB
%left MUL DIV
%right UMINUS
%right LB
%%
    line_list      : line
                   | line_list line
                   ;
       line        : expression CR  {printf("Yes\n");}
                   ;
       expression  : expression ADD expression
                   | expression SUB expression
                   | expression MUL expression
                   | expression DIV expression
                   | LB expression RB
                   | SUB  expression %prec UMINUS
                   | ID
                    ;
%%
void yyerror(char *str){
    fprintf(stderr,"error:%s\n",str);
}

int yywrap(){
    return 1;
}
int main()
{
    yyparse();
}
