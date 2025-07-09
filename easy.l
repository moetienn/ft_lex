%{ 
#include <stdio.h>
%}

DIGIT   [0-9]

%%

{DIGIT}+        { printf("NUMBER: %s\n", yytext); }
"+"|"-"        	{ printf("OPERATOR: %s\n", yytext); }
[ \t]+          { /* ignorer espaces et tabulations */ }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}