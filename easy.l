%{ 
#include <stdio.h>
%}

DIGITS	[0-9][a-z]
DIGIT   [0-9]
TEST	abc


%%

{DIGITS}+					{ printf("NUMBERS AND LETTERS: %s\n", yytext); }
{DIGIT}+					{ printf("NUMBER: %s\n", yytext); }
{TEST}+						{ printf("TESTS: %s\n", yytext); }


%%

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		yyin = fopen(argv[1], "r");
		if (!yyin)
		{
			perror(argv[1]);
			return 1;
		}
	}
	else
	{
		yyin = stdin;
	}
	
	yylex();
	
	return 0;
}
