%{
#include<stdio.h>
void print_out(int type);
void PrintStr2Bin();
/*助忆符以_开头*/
enum type {_PROGRAME=1,_BEGIN,_END,_CONST,_VAR,_WHILE,_DO,_IF,_THEN,_ID,_INT,_PLUS,_MINUS,_MULTI,_DIVI,_ASSIGN,_COMP,
           _LR,_MORE,_EQMORE,_LESS,_EQLESS,_LPAR,_RPAR,_SEPA,_COMMA};
%}


digit [0-9]
letter [a-zA-Z]
identifier {letter}({letter}|{digit})*
integer {digit}{digit}*
whitespace [ |\n|\t]*
illegal ([^a-zA-Z1-9\t\n>=<:\'\(\)\;\[\]\.\!\+\-\/\*\"\,]+)

%%
    /*单词符号及其内部表示：
    单词符号	种别编码    助忆符		内码值
    PROGRAME		1		_PROGRAME	-
    BEGIN		    2		_BEGIN		-
    END	        	3		_END		-
    CONST   		4		_CONST		-
    VAR		        5		_VAR		-
    WHILE	   		6		_WHILE		-
    DO	        	7		_DO		    -
    IF	        	8		_IF	    	-
    THEN	    	9		_THEN		-
    标识符          10		_ID		此标识符字符串
    整数            11		_INT   此常数的二进制表示
    +	          	12		_PLUS		-
    -	        	13		_MINUS		-
    *	        	14		_MULTI		-
    /		        15		_DIVI		-
   :=	        	16		_ASSIGN		-
    =	        	17		_COMP		-
    <>	        	18		_LR 		-
    >	        	19		_MORE		-
    >=	        	20		_EQMORE		-
    <	        	21		_LESS		-
    <=	        	22		_EQLESS		-
    （	        	23		_LPAR		-
    ）	        	24		_RPAR		-
    ；	        	25		_SEPA		-
    ，	        	26		_COMMA		-*/

PROGRAME     {print_out(_PROGRAME);}
BEGIN       {print_out(_BEGIN);}
END         {print_out(_END);}
CONST        {print_out(_CONST);}
VAR          {print_out(_VAR);}
WHILE        {print_out(_WHILE);}
DO            {print_out(_DO);}
IF            {print_out(_IF);}
THEN         {print_out(_THEN);}
{identifier} {
	/*检查长度是否超出范围*/
	if(yyleng > 20){
		printf("Error! The length of this idetifier is too long!\n");
		exit(0);
	}
	print_out(_ID);
}
{integer} {
    /*将整数的内码值用二进制输出*/
    printf("<11,");
    PrintStr2Bin();
	printf(">\n");
}
{whitespace} {}
 /*算符和界符*/
"+"     {print_out(_PLUS);}
"-"     {print_out(_MINUS);}
"*"     {print_out(_MULTI);}
"/"     {print_out(_DIVI);}
":="     {print_out(_ASSIGN	);}
"="     {print_out(_COMP);}
"<>"     {print_out(_LR);}
">"     {print_out(_MORE);}
">="     {print_out(_EQMORE);}
"<"     {print_out(_LESS);}
"<="     {print_out(_EQLESS);}
"("     {print_out(_LPAR);}
")"     {print_out(_RPAR);}
";"     {print_out(_SEPA);}
"," 	{print_out(_COMMA);} 
{illegal} {
	/*含有非法字符的字符串*/
	printf("<%s,error!>\n",yytext);
}

%%
void PrintStr2Bin()
{
    int num=0;
    int i=0;
    for(i=0;i<yyleng;++i){
        num=yytext[i]-'0'+num*10;
    }

    int a[100];
    for(i=0; num>0; i++)    
    {
        a[i]=num %2;
        num=(num/2);  
    }    
     
    for(i=i-1 ;i>=0 ;i--)    
    {    
        printf("%d",a[i]);    
    }
}
void print_out(int type)
{
    if(type==10){
        /*识别标识符*/
        printf("<%d,%s>\n",type,yytext);
    }
    else{
         /*识别关键字、算符和界符*/
        printf("<%d,->\n",type);
    }
}

int yywrap() 
{ 
   return(1); 
} 

int main(int argc,char** argv)
{
    yylex();
	return 0;
}