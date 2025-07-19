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
	t_nfa_state *accept = init_nfa_state(-1, false);
	t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
	if (!transition)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	transition->to = accept;
	transition->symbol = current_token->value[0];
	add_transition(start, transition);
	return (init_nfa_fragment(start, accept));
}

void	process_token_plus(t_frag_stack *frag_stack)
{
    t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
    t_nfa_state *new_accept = init_nfa_state(-1, false);

    t_nfa_transition *loop = malloc(sizeof(t_nfa_transition));
    if (!loop)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    loop->to = fragment->start;
    loop->symbol = '\0';
    add_transition(fragment->accept, loop);
    t_nfa_transition *exit = malloc(sizeof(t_nfa_transition));
    exit->to = new_accept;
    exit->symbol = '\0';
    add_transition(fragment->accept, exit);
    t_nfa_fragment *plus_fragment = init_nfa_fragment(fragment->start, new_accept);
    push_stack_frag(frag_stack, plus_fragment);
}

void	process_token_concat(t_frag_stack *frag_stack)
{
	t_nfa_fragment *right = pop_stack_frag(frag_stack);
	t_nfa_fragment *left = pop_stack_frag(frag_stack);

	t_nfa_transition *epsilon = malloc(sizeof(t_nfa_transition));
	if (!epsilon)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	epsilon->to = right->start;
	epsilon->symbol = '\0';
	add_transition(left->accept, epsilon);

	left->accept->is_accept = false;

	t_nfa_fragment *concat_fragment = init_nfa_fragment(left->start, right->accept);
	push_stack_frag(frag_stack, concat_fragment);
}
