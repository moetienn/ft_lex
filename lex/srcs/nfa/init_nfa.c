#include "../../includes/lex.h"

t_nfa_state	*init_nfa_state(int id, bool is_accept)
{
	t_nfa_state *state = malloc(sizeof(t_nfa_state));
	if (!state)
	{
		perror("Failed to allocate memory for NFA state");
		exit(EXIT_FAILURE);
	}
	state->id = id;
	state->is_accept = is_accept;
	state->transitions = NULL;
	state->transition_count = 0;
	state->transition_capacity = 0;
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

void	init_nfa(t_lex *lex)
{
    if (!lex->rpn_list)
    {
        fprintf(stderr, "Error: RPN list is not initialized\n");
        exit(EXIT_FAILURE);
    }

    t_nfa_state *start_state = init_nfa_state(0, false);
    t_nfa_state *accept_state = init_nfa_state(1, true);
    lex->nfa_frag = init_nfa_fragment(start_state, accept_state);
}
