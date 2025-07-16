#include "../../../includes/lex.h"
#include "../../../includes/nfa/nfa.h"

void    init_stack(t_stack *stack, size_t capacity)
{
    stack->data = malloc(sizeof(t_token *) * capacity);
    if (!stack->data)
    {
        perror("Memory allocation failed for operator stack");
        exit(EXIT_FAILURE);
    }
    stack->top = 0;
    stack->capacity = capacity;
}

void    push_stack(t_stack *stack, t_token *token)
{
    if (stack->top >= stack->capacity)
    {
        fprintf(stderr, "Stack overflow while pushing operator");
        exit(EXIT_FAILURE);
    }
    stack->data[stack->top++] = token;
}

t_token *pop_stack(t_stack *stack)
{
    if (stack->top == 0)
    {
        fprintf(stderr, "Stack underflow while popping operator");
        exit(EXIT_FAILURE);
    }
    return stack->data[--stack->top];
}