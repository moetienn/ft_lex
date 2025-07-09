%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

{DIGIT}+        { printf("NUMBER: %s\n", yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}