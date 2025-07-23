#include "../../includes/lex.h"

bool	is_empty_stack_frag(t_frag_stack *stack)
{
	return (stack->top == 0);
}

void free_frag_stack(t_frag_stack *stack)
{
    if (!stack)
        return;

    for (size_t i = 0; i < stack->top; i++)
    {
        if (stack->fragments[i])
        {
            free(stack->fragments[i]);
        }
    }

    free(stack->fragments);
    free(stack);
}

t_frag_stack *stack_create()
{
	t_frag_stack *stack = malloc(sizeof(t_frag_stack));
	if (!stack)
	{
		perror("Failed to allocate memory for fragment stack");
		exit(EXIT_FAILURE);
	}
	stack->fragments = NULL;
	stack->top = 0;
	stack->capacity = 0;
	return stack;
}

t_nfa_fragment *pop_stack_frag(t_frag_stack *stack)
{
	if (stack->top == 0)
	{
		fprintf(stderr, "Error: Attempt to pop from an empty stack\n");
		exit(EXIT_FAILURE);
	}
	return stack->fragments[--stack->top];
}

void    push_stack_frag(t_frag_stack *stack, t_nfa_fragment *fragment)
{
	if (stack->top >= stack->capacity)
	{
		size_t new_capacity = stack->capacity == 0 ? 1 : stack->capacity * 2;
		t_nfa_fragment **new_fragments = realloc(stack->fragments, new_capacity * sizeof(t_nfa_fragment *));
		if (!new_fragments)
		{
			perror("Failed to reallocate memory for fragment stack");
			exit(EXIT_FAILURE);
		}
		stack->fragments = new_fragments;
		stack->capacity = new_capacity;
	}
	stack->fragments[stack->top++] = fragment;
}

t_nfa_state	*create_new_state(void)
{
    t_nfa_state *state = malloc(sizeof(t_nfa_state));
    if (!state)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    state->transition_count = 0;
    state->transitions = NULL;
    state->transition_capacity = 0;
    state->is_accept = false;
    state->rule_index = -1; // Initialize rule_index to a default value
    state->visited = false; // Ensure 'visited' is initialized
    return state;
}
