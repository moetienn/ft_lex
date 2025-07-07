#ifndef LEX_H
# define LEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

typedef struct s_lex
{
    // 1 st section of the lexer file
    char    *declaration_code; // string that will contain the declarations between %{ and %}
    // 2 nd section of the lexer file
    t_parser parser;
}   t_lex;

// utils

void    free_lex(t_lex *lex);
void    file_error(const char *filename);
void	ft_lex(const char *filename);

#endif