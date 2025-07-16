#include "../../../includes/lex.h"
#include "../../../includes/nfa/nfa.h"

void append_to_rpn_list(t_token **rpn_list, t_token *operand)
{
    t_token *new_operand = malloc(sizeof(t_token)); // Create a new copy of the operand
    if (!new_operand)
    {
        perror("Memory allocation failed for RPN operand");
        exit(EXIT_FAILURE);
    }
    *new_operand = *operand; // Copy the operand's data
    new_operand->next = NULL; // Ensure the appended token does not create a circular reference

    if (!*rpn_list)
    {
        *rpn_list = new_operand; // Set as head if list is empty
    }
    else
    {
        t_token *current = *rpn_list;
        while (current->next)
        {
            current = current->next; // Traverse to the end of the list
        }
        current->next = new_operand; // Append operand to the end
    }
}

bool	priority_check(t_token *current, t_stack *operator_stack)
{
	
	if (operator_stack->top == 0)
	{
		return false; // If stack is empty, push the operator
	}

	int priority_current = 0;
	if (current->type == TOKEN_KLEENE_STAR || current->type == TOKEN_PLUS || current->type == TOKEN_OPTIONAL)
		priority_current = 3;
	else if (current->type == TOKEN_CONCAT)
		priority_current = 2;
	else if (current->type == TOKEN_ALTERNATION)
		priority_current = 1;
	
	t_token *top_operator = operator_stack->data[operator_stack->top - 1];
	int priority_top = 0;
	if (top_operator->type == TOKEN_KLEENE_STAR || top_operator->type == TOKEN_PLUS || top_operator->type == TOKEN_OPTIONAL)
		priority_top = 3;
	else if (top_operator->type == TOKEN_CONCAT)
		priority_top = 2;
	else if (top_operator->type == TOKEN_ALTERNATION)
		priority_top = 1;

	if (priority_top >= priority_current)
	{
		return true;
	}
	return false;
}

void	shunting_yard(t_lex *lex)
{
	t_stack operator_stack;

	init_stack(&operator_stack, 100); // Initialize stack with a capacity of 100

	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		t_token *current_token = lex->token_list[i];
		// printf("Processing tokens for rule %zu:\n", i);
		while (current_token)
		{
			if (is_operand(current_token->type))
				append_to_rpn_list(&lex->rpn_list[i], current_token); // Append operand to RPN list
			else if (is_operator(current_token->type))
			{
				// printf("Processing operator: %s\n", current_token->value);
				if(priority_check(current_token, &operator_stack) == false)
					push_stack(&operator_stack, current_token); // Push operator onto stack
				else
				{
					if (operator_stack.top == 0)
						append_to_rpn_list(&lex->rpn_list[i], current_token); // Append operator to RPN list
					else
					{
						t_token *top_operator = pop_stack(&operator_stack); // Pop operator from stack
						push_stack(&operator_stack, current_token); // Push current operator onto stack
						append_to_rpn_list(&lex->rpn_list[i], top_operator); // Append popped operator to RPN list
					}
				}
			}
			current_token = current_token->next; // Move to the next token in the linked list
		}
		// Pop all operators from the stack to the RPN list
		while (operator_stack.top > 0)
		{
			t_token *operator = pop_stack(&operator_stack);
			append_to_rpn_list(&lex->rpn_list[i], operator);
		}
	}
	free(operator_stack.data);
	// print RPN list for debugging
	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		t_token *current = lex->rpn_list[i];
		printf("RPN List %zu: ", i);
		while (current)
		{
			printf("%s ", current->value);
			current = current->next;
		}
		printf("\n");
	}
}

void	rpn(t_lex *lex)
{
	printf("Converting tokenized patterns to RPN...\n");
	// Implement RPN conversion logic here
	shunting_yard(lex);
}