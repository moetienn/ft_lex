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
bool    is_whitespace(char c);
void    free_lex(t_lex *lex);
void    file_error(const char *filename);

// Macros
void    collect_macros(t_lex *lex, size_t *i);
// Rules
char    *extract_action(const char *str, size_t *i);

char	*extract_pattern(const char *str, size_t *i);
char    *replace_tmp_with_macro(const char *tmp, t_macros_list *macros_list);
char	*append_macro(const char *str, size_t *i, char *macro, size_t start);
char	*pattern_to_macro(const char *str, size_t *i, t_macros_list *macros_list);
void    collect_rules(t_lex *lex);
//

void	ft_lex(const char *filename);

#endif