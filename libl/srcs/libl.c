#include "../includes/libl.h"

char*	yytext = NULL;  // Pointer to the current token text
int		yyleng = 0;       // Length of the current token text
FILE*	yyin = NULL;    // Input file pointer
FILE*	yyout = NULL;   // Output file pointer
int		yy_more_flag = 0; // Flag to indicate if more text will be appended to yytext

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

// Read the next character from the input stream
int input(void)
{
	int	c;
	FILE *stream = yyin;
	if (!stream)
		stream = stdin; // Default to standard input if yyin is NULL
	c = fgetc(stream); // Read a character from the input stream
	if (c == EOF)
		return (0); // Return 0 on end of file
	return (c);
}

// Push back a character to the input stream
int unput(int c)
{
	FILE *stream = yyin;

	if (!stream)
  		stream = stdin; // Default to standard input if yyin is NULL
	if (c != 0)
		ungetc(c, stream); // Push back the character to the input stream
	return (c);
}

int yywrap(void)
{
	// This function is called when the end of the input stream is reached
	return (1);
}
