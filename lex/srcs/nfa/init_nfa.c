#include "../../includes/lex.h"

t_nfa_state	*init_nfa_state(int id, bool is_accept)
{
	(void)id;
	t_nfa_state *state = malloc(sizeof(t_nfa_state));
	if (!state)
	{
		perror("Failed to allocate memory for NFA state");
		exit(EXIT_FAILURE);
	}
	state->id = g_nfa_state_id++;
	printf("Initializing NFA state with ID: %d\n", state->id);
	state->is_accept = is_accept;
	state->transitions = NULL;
	state->transition_count = 0;
	state->transition_capacity = 0;
	state->rule_index = -1;
	state->visited = false;
	return state;
}

t_nfa_fragment	*init_nfa_fragment(t_nfa_state *start, t_nfa_state *accept)
{
	t_nfa_fragment *fragment = malloc(sizeof(t_nfa_fragment));
	if (!fragment)
	{
		perror("Failed to allocate memory for NFA fragment");
		exit(EXIT_FAILURE);
	}
	fragment->start = start;
	fragment->accept = accept;
	return fragment;
}
