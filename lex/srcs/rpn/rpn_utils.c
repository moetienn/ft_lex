#include "../../includes/lex.h"
#include "../../includes/nfa/nfa.h"

t_token	*create_token_copy(t_token *operand)
{
    t_token *new_operand = malloc(sizeof(t_token));
    if (!new_operand)
    {
        perror("Memory allocation failed for RPN operand");
        exit(EXIT_FAILURE);
    }
    new_operand->type = operand->type;
    new_operand->value = operand->value ? strdup(operand->value) : NULL;
    if (operand->value && !new_operand->value)
    {
        perror("Memory allocation failed for RPN operand value");
        exit(EXIT_FAILURE);
    }
    new_operand->next = NULL;
    return new_operand;
}

void	append_to_rpn_list(t_token **rpn_list, t_token *operand)
{
    t_token *new_operand = create_token_copy(operand);

    if (!*rpn_list)
    {
        *rpn_list = new_operand; // Set as head if list is empty
        return;
    }

    t_token *current = *rpn_list;
    while (current->next)
    {
        current = current->next; // Traverse to the end of the list
    }
    current->next = new_operand; // Append operand to the end
}
