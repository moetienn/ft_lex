#include "../../includes/lex.h"
#include <stdint.h>

t_nfa_state *stack_pop(PtrStack *stack)
{
	if (!stack || stack->size == 0)
		return NULL;

	t_nfa_state *state = stack->items[--stack->size];
	stack->items[stack->size] = NULL;
	return state;
}

void	stack_push(PtrStack *stack, t_nfa_state *state)
{
	if (!stack || !state)
		return;

	if (stack->size >= stack->capacity)
	{
		size_t new_capacity = stack->capacity == 0 ? 1 : stack->capacity * 2;
		t_nfa_state **new_items = realloc(stack->items, new_capacity * sizeof(t_nfa_state *));
		if (!new_items)
		{
			perror("Failed to reallocate memory for stack");
			exit(EXIT_FAILURE);
		}
		stack->items = new_items;
		stack->capacity = new_capacity;
	}
	stack->items[stack->size++] = state;
}

bool	stack_contains(PtrStack *stack, t_nfa_state *state)
{
	if (!stack || !state || stack->size == 0)
		return false;

	for (size_t i = 0; i < stack->size; i++)
	{
		if (stack->items[i] == state)
			return true;
	}
	return false;
}

void free_nfa_state_iterative(t_nfa_state *start)
{
    PtrStack to_visit = { NULL, 0, 0 };
    PtrStack visited = { NULL, 0, 0 };

    stack_push(&to_visit, start);

    while (to_visit.size > 0) {
        t_nfa_state *state = stack_pop(&to_visit);

        if (!state || stack_contains(&visited, state))
            continue;
        stack_push(&visited, state);

        for (size_t i = 0; i < state->transition_count; i++) {
            if (state->transitions[i]->to)
                stack_push(&to_visit, state->transitions[i]->to);
            free(state->transitions[i]);
        }
        free(state->transitions);
        free(state);
    }
	free(visited.items);
	free(to_visit.items);
}
