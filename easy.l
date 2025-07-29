%{ 
#include <stdio.h>
%}

DIGIT   [0-9]
LETTER  [a-z]

%%

{DIGIT}+        	{ printf("NUMBER: %s\n", yytext); }
{LETTER}*			{ printf("FOUND %zu letter(s): \"%s\"\n", yyleng, yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}