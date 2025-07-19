%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

a+           { printf("MATCH a+: %s\n", yytext); }
a+b            { printf("MATCH a+b: %s\n", yytext); }



%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}