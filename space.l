   	%{ 							
 #include <stdio.h>
#include <stdlib.h>
	%}
													
DIGIT   [0-9]
ID      [A-Za-z_][A-Za-z0-9_]*

    %%  													

		{DIGIT}+        { printf("NUMBER: %s\n", yytext); }
    " + "|" - "         { printf("OPERATOR: %s\n", yytext); }
  [ \t]+          { /* ignorer espaces et tabulations */ }
	\n              { /* compter ou traiter les retours à la ligne */ }
 .               { printf("OTHER: %c\n", *yytext); }

 					%%     											

	int main(void)
				{
			/* yyin et yyout sont gérés par libl */
				return yylex();
			}
