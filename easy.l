%{ 
#include <stdio.h>
%}

DIGIT   [0-9

%%

{DIGIT}+				{ printf("NUMBER: %s\n", yytext); }
"+"|"-"|"*"				{ printf("OPERATOR: %s\n", yytext); }


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
