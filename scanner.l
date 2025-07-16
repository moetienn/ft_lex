%{ 
#include <stdio.h>
%}

DIGIT   [0-9]
ID      [A-Za-z_][A-Za-z0-9_]*
LETTER  [a-z]

%%

{DIGIT}+        	{ printf("NUMBER: %s\n", yytext); }
"+"|"-"         	{ printf("OPERATOR: %s\n", yytext); }
[ \t]+          	{ /* ignorer espaces et tabulations */ }
\n              	{ /* compter ou traiter les retours à la ligne */ }			
.               	{ printf("OTHER: %c\n", *yytext); }
{LETTER}*			{ printf("FOUND %zu letter(s): \"%s\"\n", yyleng, yytext); }
[0-9]?				{ printf("OPTIONAL_DIGIT: %s\n", yytext); }
[0-9]+X         { printf("DIGITS then X: %s\n", yytext); }
(if|else)[0-9]  { printf("KEYWORD+digit: %s\n", yytext); }
A*B             { printf("zero-or-more A then B: %s\n", yytext); }
(ab)?c          { printf("optional ab then c: %s\n", yytext); }

%%

int main(void)
{
	/* yyin et yyout sont gérés par libl */
	return yylex();
}
