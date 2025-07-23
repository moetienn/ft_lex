#include "../../includes/lex.h"

void	add_transition(t_nfa_state *state, t_nfa_transition *transition)
{
	if (!state)
	{
		printf("[TRACE] add_transition: state is NULL, skipping.\n");
		return;
	}
	printf("[TRACE] add_transition: Adding transition %p to state %p\n", (void*)transition, (void*)state);
	if (state->transition_count == state->transition_capacity)
	{
		state->transition_capacity = state->transition_capacity == 0 ? 1 : state->transition_capacity * 2;
		printf("[TRACE] add_transition: Reallocating transitions for state %p\n", (void*)state);
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
	printf("[TRACE] add_transition: Transition %p added to state %p\n", (void*)transition, (void*)state);
}

t_nfa_fragment	*process_token_char(t_token *current_token)
{
    t_nfa_state *start = init_nfa_state(-1, false);
    printf("[TRACE] process_token_char: Created start state %p\n", (void*)start);
    t_nfa_state *accept = init_nfa_state(-1, false);
    printf("[TRACE] process_token_char: Created accept state %p\n", (void*)accept);
    t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
    printf("[TRACE] process_token_char: Created transition %p\n", (void*)transition);
    if (!transition)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    transition->to = accept;
    printf("[TRACE] process_token_char: Setting transition to accept state %p\n", (void*)accept);
    transition->symbol = current_token->value[0];
    add_transition(start, transition);
    t_nfa_fragment *fragment = init_nfa_fragment(start, accept);
    printf("[TRACE] process_token_char: Created fragment with start state %p and accept state %p\n", (void*)start, (void*)accept);
    return fragment;
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
    t_nfa_transition *exit_transition = malloc(sizeof(t_nfa_transition));
    if (!exit_transition)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    exit_transition->to = new_accept;
    exit_transition->symbol = '\0';
    add_transition(fragment->accept, exit_transition);
    t_nfa_fragment *plus_fragment = init_nfa_fragment(fragment->start, new_accept);
    push_stack_frag(frag_stack, plus_fragment);
    free(fragment);
    fragment = NULL;
}

void process_token_kleene_star(t_frag_stack *frag_stack)
{
    t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
    t_nfa_state *new_start = init_nfa_state(-1, false);
    printf("[TRACE] process_token_kleene_star: Created new start state %p\n", (void*)new_start);
    t_nfa_state *new_accept = init_nfa_state(-1, false);
    printf("[TRACE] process_token_kleene_star: Created new accept state %p\n", (void*)new_accept);
    t_nfa_transition *loop = malloc(sizeof(t_nfa_transition));
    printf("[TRACE] process_token_kleene_star: Created loop transition %p\n", (void*)loop);
    if (!loop)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    loop->to = fragment->start;
    printf("[TRACE] process_token_kleene_star: Setting loop transition to start state %p\n", (void*)fragment->start);
    loop->symbol = '\0';
    add_transition(fragment->accept, loop);

    t_nfa_transition *exit_tr = malloc(sizeof(t_nfa_transition));
    printf("[TRACE] process_token_kleene_star: Created exit transition %p\n", (void*)exit_tr);
    if (!exit_tr)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    exit_tr->to = new_accept;
    printf("[TRACE] process_token_kleene_star: Setting exit transition to new accept state %p\n", (void*)new_accept);
    exit_tr->symbol = '\0';
    add_transition(fragment->accept, exit_tr);
    t_nfa_transition *enter = malloc(sizeof(t_nfa_transition));
    printf("[TRACE] process_token_kleene_star: Created enter transition %p\n", (void*)enter);
    if (!enter)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    enter->to = fragment->start;
    printf("[TRACE] process_token_kleene_star: Setting enter transition to fragment start state %p\n", (void*)fragment->start);
    enter->symbol = '\0';
    add_transition(new_start, enter);
    t_nfa_transition *empty = malloc(sizeof(t_nfa_transition));
    printf("[TRACE] process_token_kleene_star: Created empty transition %p\n", (void*)empty);
    if (!empty)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    empty->to = new_accept;
    printf("[TRACE] process_token_kleene_star: Setting empty transition to new accept state %p\n", (void*)new_accept);
    empty->symbol = '\0';
    add_transition(new_start, empty);
    t_nfa_fragment *star_fragment = init_nfa_fragment(new_start, new_accept);
    printf("[TRACE] process_token_kleene_star: Created star fragment %p\n", (void*)star_fragment);
    push_stack_frag(frag_stack, star_fragment);
    free(fragment);
    fragment = NULL;
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
    free(left);
    free(right);
}
