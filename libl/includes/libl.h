#ifndef LIBL_H
# define LIBL_H

#include <stdio.h>

extern int yy_more_flag; // Flag to indicate more text will be appended

extern char *yytext;	// Pointer to the current token text
extern int  yyleng;		// Length of the current token text
extern FILE *yyin;		// Input file pointer
extern FILE *yyout;		// Output file pointer

//  Function prototypes for the libl library

int   yymore(void);
int   yyless(int n);
int   input(void);
int   output(void);
int   yywrap(void);
int   unput(int c);

#endif