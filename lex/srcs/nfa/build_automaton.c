#include "../../includes/lex.h"

char decode_escape(char c)
{
    switch (c)
    {
        case 'n': return '\n';
        case 't': return '\t';
        case 'r': return '\r';
        case 'b': return '\b';
        case 'f': return '\f';
        case '\\': return '\\';
        case '\'': return '\'';
        case '"': return '"';
        default: return c;
    }
}

t_nfa_fragment	*process_token_escape(t_token *current_token)
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
    transition->symbol = decode_escape(current_token->value[1]);
    add_transition(start, transition);
    t_nfa_fragment *fragment = init_nfa_fragment(start, accept);
    return fragment;
}

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
    t_nfa_fragment *fragment = init_nfa_fragment(start, accept);
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

static t_nfa_transition *create_transition(t_nfa_state *to, char symbol)
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

static void add_loop_transition(t_nfa_state *accept, t_nfa_state *start)
{
    t_nfa_transition *loop = create_transition(start, '\0');
    add_transition(accept, loop);
}

static void add_exit_transition(t_nfa_state *accept, t_nfa_state *new_accept)
{
    t_nfa_transition *exit_tr = create_transition(new_accept, '\0');
    add_transition(accept, exit_tr);
}

static void add_enter_transition(t_nfa_state *new_start, t_nfa_state *start)
{
    t_nfa_transition *enter = create_transition(start, '\0');
    add_transition(new_start, enter);
}

static void add_empty_transition(t_nfa_state *new_start, t_nfa_state *new_accept)
{
    t_nfa_transition *empty = create_transition(new_accept, '\0');
    add_transition(new_start, empty);
}

void    process_token_optional(t_frag_stack *frag_stack)
{
    t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
    t_nfa_state *new_start = init_nfa_state(-1, false);
    t_nfa_state *new_accept = init_nfa_state(-1, false);


    add_enter_transition(new_start, fragment->start);
    add_exit_transition(fragment->accept, new_accept);
    add_empty_transition(new_start, new_accept);

    t_nfa_fragment *optional_fragment = init_nfa_fragment(new_start, new_accept);
    push_stack_frag(frag_stack, optional_fragment);
    free(fragment);
}

void	process_token_alternation(t_frag_stack *frag_stack)
{
    t_nfa_fragment *fragment_b = pop_stack_frag(frag_stack);
    t_nfa_fragment *fragment_a = pop_stack_frag(frag_stack);

    t_nfa_state *new_start = init_nfa_state(-1, false);
    t_nfa_state *new_accept = init_nfa_state(-1, false);
    add_enter_transition(new_start, fragment_a->start);
    add_enter_transition(new_start, fragment_b->start);
    add_exit_transition(fragment_a->accept, new_accept);
    add_exit_transition(fragment_b->accept, new_accept);
    t_nfa_fragment *alt_fragment = init_nfa_fragment(new_start, new_accept);
    push_stack_frag(frag_stack, alt_fragment);
    free(fragment_a);
    free(fragment_b);
}

void process_token_kleene_star(t_frag_stack *frag_stack)
{
    t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
    t_nfa_state *new_start = init_nfa_state(-1, false);
    t_nfa_state *new_accept = init_nfa_state(-1, false);

    add_loop_transition(fragment->accept, fragment->start);
    add_exit_transition(fragment->accept, new_accept);
    add_enter_transition(new_start, fragment->start);
    add_empty_transition(new_start, new_accept);

    t_nfa_fragment *star_fragment = init_nfa_fragment(new_start, new_accept);
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
