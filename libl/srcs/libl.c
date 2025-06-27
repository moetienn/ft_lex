#include "../includes/libl.h"

char*	yytext = NULL;  // Pointer to the current token text
int		yyleng = 0;       // Length of the current token text
FILE*	yyin = NULL;    // Input file pointer
FILE*	yyout = NULL;   // Output file pointer

// Append the next lexeme to yytext rather than replacing it
int yymore(void)
{
	yy_more_flag = 1; // Set the flag to indicate more text will be appended
	return (0);
}

// Keep only the first n characters of yytext and push back the rest
int yyless(int n)
{
	if (!yytext || yyleng <= 0)
		return (0); // If yytext is NULL or empty, do nothing

	int	i;

	if (n > yyleng)
		n = yyleng; // Ensure n does not exceed the current length
	i = yyleng - 1;
	while (i >= n)
	{
		unput(yytext[i]); // Push back characters to the input stream
		i--;
	}
	yytext[n] = '\0'; // Null-terminate the modified string
	yyleng = n;
	yy_more_flag = 0; // Reset the flag
	return (0);
}

int input(void)
{
	return (0);
}

int output(void)
{
	return (0);
}

int yywrap(void)
{
	return (0);
}

int unput(int c)
{
	return (0);
}