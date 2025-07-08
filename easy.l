%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

"+"|"-"         { printf("OPERATOR: %s\n", yytext); }
[ \t]+          { /* ignorer espaces et tabulations */ }
\n              { /* compter ou traiter les retours à la ligne */ }
.               { printf("OTHER: %c\n", *yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}