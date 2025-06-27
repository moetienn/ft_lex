#ifndef LIBL_H
# define LIBL_H

#include <stdio.h>

int   yymore(void);
int   yyless(int n);
int   input(void);
int   output(void);
int   yywrap(void);
int   unput(int c);

#endif