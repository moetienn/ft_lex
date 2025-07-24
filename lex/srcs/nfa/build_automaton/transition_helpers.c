#include "../../../includes/lex.h"

void	add_transition(t_nfa_state *state, t_nfa_transition *transition)
{
	if (!state)
		return;
	if (state->transition_count == state->transition_capacity)
	{
		state->transition_capacity = state->transition_capacity == 0 ? 1 : state->transition_capacity * 2;
		if (!state->transitions)
		{
			state->transitions = malloc(state->transition_capacity * sizeof(t_nfa_transition *));
			if (!state->transitions)
			{
				perror("Failed to allocate memory for transitions");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			t_nfa_transition **new_transitions = realloc(state->transitions, state->transition_capacity * sizeof(t_nfa_transition *));
			if (!new_transitions)
			{
				perror("Failed to reallocate memory for transitions");
				exit(EXIT_FAILURE);
			}
			state->transitions = new_transitions;
		}
	}
	state->transitions[state->transition_count++] = transition;
}

t_nfa_transition *create_transition(t_nfa_state *to, char symbol)
{
	t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
	if (!transition)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	transition->to = to;
	transition->symbol = symbol;
	return transition;
}

void add_loop_transition(t_nfa_state *accept, t_nfa_state *start)
{
	t_nfa_transition *loop = create_transition(start, '\0');
	add_transition(accept, loop);
}

void add_exit_transition(t_nfa_state *accept, t_nfa_state *new_accept)
{
	t_nfa_transition *exit_tr = create_transition(new_accept, '\0');
	add_transition(accept, exit_tr);
}

void add_enter_transition(t_nfa_state *new_start, t_nfa_state *start)
{
	t_nfa_transition *enter = create_transition(start, '\0');
	add_transition(new_start, enter);
}

void add_empty_transition(t_nfa_state *new_start, t_nfa_state *new_accept)
{
	t_nfa_transition *empty = create_transition(new_accept, '\0');
	add_transition(new_start, empty);
}
