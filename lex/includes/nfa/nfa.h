#ifndef NFA_H
# define NFA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum e_token_type
{
	TOKEN_CHAR,            // single character (literal)
	TOKEN_CLASS,           // character class [ ... ]
	TOKEN_QUOTE,           // quoted string "..."
	TOKEN_ESCAPE,          // escape sequence \x
	TOKEN_CONCAT,          // explicit concatenation operator ·
	TOKEN_ALTERNATION,     // |
	TOKEN_KLEENE_STAR,     // *
	TOKEN_PLUS,            // +
	TOKEN_OPTIONAL,        // ?
	TOKEN_LPAREN,          // (
	TOKEN_RPAREN,          // )
	TOKEN_QUANTIFIER,      // {m} or {m,n}
	TOKEN_END              // end-of-input marker
}   t_token_type;

typedef struct s_token {
	t_token_type type;
	char *value; // For literals, this will hold the actual string
	struct s_token *next; // For linked list of tokens
}   t_token;

// utils
void			add_next_token(t_token **current, t_token_type type, const char *value);
void			initialize_first_token(t_token **token_list, t_token **current, t_token_type type);
t_token_type	get_token_type(char c);
void			free_token_list(t_token **token_list);
void			init_token_list(t_token ***token_list, size_t count);

#endif