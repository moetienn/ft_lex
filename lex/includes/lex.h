#ifndef LEX_H
# define LEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "declaration/macros.h"
#include "rules/rules.h"


typedef struct s_lex
{
    // 1 st section of the lexer file
    char    *declaration_code; // string that will contain the declarations between %{ and %}
    t_macros_list macros_list; // list of macros
    // 2 nd section of the lexer file
    t_parser parser;
    t_rules_list rules_list;
}   t_lex;

// utils

// Macros
void    collect_macros(t_lex *lex, size_t *i);\
// Rules
void    collect_rules(t_lex *lex);
//

void    free_lex(t_lex *lex);
void    file_error(const char *filename);
void	ft_lex(const char *filename);

#endif