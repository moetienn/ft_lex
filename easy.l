%{ 
#include <stdio.h>
%}

DIGIT   [0-9]
LETTER  [a-z]

%%

{LETTER}*			{ printf("FOUND %u letter(s): \"%s\"\n", yyleng, yytext); }
{DIGIT}+        	{ printf("NUMBER: %s\n", yytext); }

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