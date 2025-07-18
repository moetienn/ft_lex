%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

ab              { printf("MATCH ab\n"); }
a*b        { printf("A*B: %s\n", yytext); }
(ab)*      { printf("(AB)*: %s\n", yytext); }
a*b|(ab)*      { printf("a* b or (ab)*: %s\n", yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}