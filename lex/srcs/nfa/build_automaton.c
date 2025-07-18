#include "../../includes/lex.h"

void	add_transition(t_nfa_state *state, t_nfa_transition *transition)
{
	if (state->transition_count == state->transition_capacity)
	{
		state->transition_capacity = state->transition_capacity == 0 ? 1 : state->transition_capacity * 2;
		state->transitions = realloc(state->transitions, state->transition_capacity * sizeof(t_nfa_transition *));
		if (!state->transitions)
		{
			perror("Failed to reallocate memory for transitions");
			exit(EXIT_FAILURE);
		}
	}
	state->transitions[state->transition_count++] = transition;
}

t_nfa_fragment	*process_token_char(t_token *current_token)
{
	t_nfa_state *start = init_nfa_state(-1, false);
	t_nfa_state *accept = init_nfa_state(-1, true);
	t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
	if (!transition)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	transition->to = accept;
	transition->symbol = current_token->value[0];
	add_transition(start, transition);
	// printf("[process_token_char] Create fragment: start id=%d, accept id=%d, symbol='%c'\n",
        // start->id, accept->id, current_token->value[0]);
	return init_nfa_fragment(start, accept);
}
