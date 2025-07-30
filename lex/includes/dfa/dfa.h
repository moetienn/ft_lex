#ifndef DFA_H
# define DFA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../nfa/nfa.h"

extern int g_dfa_state_id;

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
	int                 action_id;
}	t_dfa_state;

typedef struct s_dfa
{
	t_dfa_state	*start_state;
	t_dfa_state	**accept_states;
	int			accept_count;
	t_dfa_state	**states;
	int			state_count;
}	t_dfa;

typedef struct s_worklist
{
	t_dfa_state	**states;
	int			size;
	int			capacity;
}	t_worklist;

void	collect_alphabet_from_nfa(t_nfa_state *super_start, char *alphabet, int *alphabet_size, int max_state_id);

// Worklist functions
void	init_worklist(t_worklist *wl);
void	worklist_push(t_worklist *wl, t_dfa_state *state);
t_dfa_state	*worklist_pop(t_worklist *wl);

// EPSILON
void	epsilon_closure(t_nfa_state *start, t_nfa_state **closure, int *closure_size, bool *visited);
void	calculate_epsilon_closure(t_nfa_state **reachable_states, int reachable_count, t_nfa_state ***closure, int *closure_size);

// STATE
void	find_reachable_states(t_dfa_state *current_state, char symbol, t_nfa_state ***reachable_states, int *reachable_count);
t_dfa_state *find_or_create_next_state(t_nfa_state **closure, int closure_size, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist, t_dfa *dfa);

#endif