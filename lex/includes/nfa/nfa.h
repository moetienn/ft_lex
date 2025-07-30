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

extern int g_nfa_state_id;

typedef struct s_nfa_transition t_nfa_transition;

typedef struct s_nfa_state
{
	int				id;
	int				rule_index;
	bool			is_accept;
	t_nfa_transition	**transitions;
	size_t			transition_count;
	size_t			transition_capacity;
	bool			visited;
}	t_nfa_state;

typedef struct s_nfa_transition
{
	t_nfa_state	*to;
	char		symbol;
}	t_nfa_transition;

typedef struct s_nfa_fragment
{
	t_nfa_state		*start;
	t_nfa_state		*accept;
}	t_nfa_fragment;

typedef struct s_frag_stack
{
	t_nfa_fragment	**fragments;
	size_t			top;
	size_t			capacity;
}	t_frag_stack;

typedef struct {
    t_nfa_state **items;
    size_t size;
    size_t capacity;
}	PtrStack;

bool	stack_contains(PtrStack *stack, t_nfa_state *state);
void	stack_push(PtrStack *stack, t_nfa_state *state);
t_nfa_state	*stack_pop(PtrStack *stack);
void	free_nfa_state_iterative(t_nfa_state *start);

// utils
void			add_next_token(t_token **current, t_token_type type, const char *value);
void			initialize_first_token(t_token **token_list, t_token **current, t_token_type type);
t_token_type	get_token_type(char c);
void			free_token_list(t_token **token_list, size_t count);
void			init_token_list(t_token ***token_list, size_t count);
bool			is_empty_stack_frag(t_frag_stack *stack);
void			free_frag_stack(t_frag_stack *stack);

// transition helper
void	add_transition(t_nfa_state *state, t_nfa_transition *transition);
t_nfa_transition *create_transition(t_nfa_state *to, char symbol);
void	add_loop_transition(t_nfa_state *accept, t_nfa_state *start);
void	add_exit_transition(t_nfa_state *accept, t_nfa_state *new_accept);
void	add_enter_transition(t_nfa_state *new_start, t_nfa_state *start);
void	add_empty_transition(t_nfa_state *new_start, t_nfa_state *new_accept);

// process nfa token
void	add_transition(t_nfa_state *state, t_nfa_transition *transition);
void	process_token_plus(t_frag_stack *frag_stack, int rule_index);
void	process_token_concat(t_frag_stack *frag_stack);
void	process_token_kleene_star(t_frag_stack *frag_stack, int rule_index);
void	process_token_alternation(t_frag_stack *frag_stack, int rule_index);
void	process_token_optional(t_frag_stack *frag_stack, int rule_index);
void	process_token_quantifier(t_frag_stack *frag_stack, t_token *current_token, int rule_index);
t_nfa_fragment	*process_token_char(t_token *current_token, int rule_index);
t_nfa_fragment	*process_token_escape(t_token *current_token, int rule_index);
t_nfa_fragment	*process_token_class(t_token *current_token, int rule_index);

#endif