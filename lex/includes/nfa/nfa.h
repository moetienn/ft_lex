#ifndef NFA_H
# define NFA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TOKEN

typedef enum e_token_type
{
	TOKEN_CHAR,            // single character (literal)
	TOKEN_CLASS,           // character class [ ... ]
	TOKEN_QUOTE,           // quoted string "..."
	TOKEN_ESCAPE,          // escape sequence \x or \n or \t etc.
	TOKEN_CONCAT,          // explicit concatenation operator ·
	TOKEN_ALTERNATION,     // |
	TOKEN_KLEENE_STAR,     // *
	TOKEN_PLUS,            // +
	TOKEN_OPTIONAL,        // ?
	TOKEN_LPAREN,          // (
		TOKEN_RPAREN,          // )
		TOKEN_QUANTIFIER,      // {m} or {m,n}
	}   t_token_type;
	
typedef struct s_token
{
	t_token_type	type;
	char			*value; // For literals, this will hold the actual string
	struct s_token	*next; // For linked list of tokens
}	t_token;
	
typedef struct s_stack
{
	t_token	**data;
	size_t	top;
	size_t	capacity;
}	t_stack;

// NFA

typedef struct s_nfa_transition t_nfa_transition;

typedef struct s_nfa_state
{
	int				id;
	bool			is_accept;
	t_nfa_transition	**transitions; // Array of pointers to transition structs
	size_t			transition_count;
	size_t			transition_capacity; // Capacity of the transitions array
	
}	t_nfa_state;

typedef struct s_nfa_transition
{
	t_nfa_state	*to;   // To state
	char		symbol; // Transition symbol (for character transitions)
}	t_nfa_transition;

typedef struct s_nfa_fragment
{
	t_nfa_state		*start;
	t_nfa_state		*accept;
}	t_nfa_fragment;

// utils
void			add_next_token(t_token **current, t_token_type type, const char *value);
void			initialize_first_token(t_token **token_list, t_token **current, t_token_type type);
t_token_type	get_token_type(char c);
void			free_token_list(t_token **token_list, size_t count);
void			init_token_list(t_token ***token_list, size_t count);

#endif