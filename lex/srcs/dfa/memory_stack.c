#include "../../includes/dfa/dfa.h"

void	update_memory_stack(t_memory_stack *stack, void *old_ptr, void *new_ptr)
{
	for (int i = 0; i <= stack->top; ++i)
	{
		if (stack->pointers[i] == old_ptr)
		{
			stack->pointers[i] = new_ptr;
			return;
		}
	}
    if (old_ptr == NULL)
	{
		stack->pointers[stack->top + 1] = new_ptr;
		stack->top++;
	}
}

t_memory_stack *init_memory_stack(int capacity)
{
    t_memory_stack *stack = malloc(sizeof(t_memory_stack));
    if (!stack)
	{
        fprintf(stderr, "Error: Failed to allocate memory for memory stack\n");
        return NULL;
    }
    stack->pointers = malloc(sizeof(void *) * capacity);
    if (!stack->pointers)
	{
        fprintf(stderr, "Error: Failed to allocate memory for stack pointers\n");
        free(stack);
        return NULL;
    }
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void	push_memory_stack(t_memory_stack *stack, void *ptr)
{
    if (stack->top == stack->capacity - 1)
    {
        // realloc memory stack if full
        void **new_pointers = realloc(stack->pointers, sizeof(void *) * (stack->capacity * 2));
        if (!new_pointers)
        {
            fprintf(stderr, "Error: Failed to reallocate memory for memory stack pointers\n");
            return;
        }
        stack->pointers = new_pointers;
        stack->capacity *= 2;
    }
    for (int i = 0; i <= stack->top; ++i)
    {
        if (stack->pointers[i] == ptr)
        {
            return;
        }
    }
    stack->pointers[++stack->top] = ptr;
}

void	free_memory_stack(t_memory_stack *stack)
{
    if (!stack) return;
    for (int i = 0; i <= stack->top; ++i)
	{
		if (stack->pointers[i] == NULL)
			continue;
        free(stack->pointers[i]);
		stack->pointers[i] = NULL;
    }
    free(stack->pointers);
    free(stack);
}
