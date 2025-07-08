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
	TOKEN_MACRO,           // macro reference {NAME}
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
void			init_token(t_token **token);
void			free_token(t_token *head);
void			add_token(t_token **head, t_token	*tok);
struct s_token	*token_new(t_token_type type, const char *value);

#endif