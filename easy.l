%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

a\tb				{ printf("MATCH a?: %s\n", yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}