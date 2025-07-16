%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

ab              { printf("MATCH ab\n"); }
A*B             { printf("zero-or-more A then B: %s\n", yytext); }
[0-9]+X         { printf("DIGITS then X: %s\n", yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}