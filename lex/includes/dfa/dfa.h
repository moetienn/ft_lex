#ifndef DFA_H
# define DFA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../nfa/nfa.h"

typedef struct s_dfa_transition
{
	char				symbol;
	struct s_dfa_state	*next_state;
}	t_dfa_transition;

typedef struct s_dfa_state
{
	int					id;
	bool				is_accept;
	t_dfa_transition	**transitions;
	int					transition_count;
	t_nfa_state			**nfa_states;
	int					nfa_state_count;
}	t_dfa_state;

typedef struct s_dfa
{
	t_dfa_state	*start_state;
	t_dfa_state	**accept_states;
	int			accept_count;
	t_dfa_state	**states;
	int			state_count;
}	t_dfa;

#endif