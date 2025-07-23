%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

a|b|c				{ printf("MATCH a*b: %s\n", yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}