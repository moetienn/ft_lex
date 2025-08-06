#include "../../includes/lex.h"
#include "../../includes/nfa/nfa.h"

void	handle_operator(t_token *current_token, t_stack *operator_stack, t_token **rpn_list)
{
	if (priority_check(current_token, operator_stack) == false)
	{
		// printf("Pushing operator to stack: %s\n", current_token->value);
		push_stack(operator_stack, current_token);
	}
	else
	{
		if (operator_stack->top == 0)
			append_to_rpn_list(rpn_list, current_token);
		else if (current_token->type == TOKEN_RPAREN)
		{
			while (operator_stack->top > 0 && operator_stack->data[operator_stack->top - 1]->type != TOKEN_LPAREN)
			{
				t_token *top_operator = pop_stack(operator_stack);
				append_to_rpn_list(rpn_list, top_operator);
			}
		}
		else
		{
			t_token *top_operator = pop_stack(operator_stack);
			push_stack(operator_stack, current_token);
			append_to_rpn_list(rpn_list, top_operator);
		}
	}
}

void	pop_remaining_operators(t_stack *operator_stack, t_token **rpn_list)
{
	while (operator_stack->top > 0)
	{
		if (operator_stack->top > 1 && operator_stack->data[operator_stack->top - 1]->type == TOKEN_PLUS)
		{
			t_token *below_operator = operator_stack->data[operator_stack->top - 2];
			if (below_operator->type == TOKEN_CONCAT)
			{
				t_token *tmp = pop_stack(operator_stack);
				operator_stack->top--; // Remove TOKEN_CONCAT manually
				// printf("Popping TOKEN_CONCAT: %s\n", below_operator->value);
				append_to_rpn_list(rpn_list, below_operator);
				append_to_rpn_list(rpn_list, tmp);
				continue; // Skip popping the top in this iteration
			}
		}

		t_token *operator = pop_stack(operator_stack);
		// printf("Popping operator from stack: %s\n", operator->value);
		if (operator->type != TOKEN_LPAREN && operator->type != TOKEN_RPAREN)
		{
			append_to_rpn_list(rpn_list, operator);
		}
	}
}

void	shunting_yard(t_lex *lex)
{
	t_stack	operator_stack;

	init_stack(&operator_stack, 100);

	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		t_token *current_token = lex->token_list[i];
		while (current_token)
		{
			// printf("Processing token: %s\n", current_token->value);
			if (is_operand(current_token->type))
			{
				// printf("Appending operand: %s\n", current_token->value);
				append_to_rpn_list(&lex->rpn_list[i], current_token);
			}
			else if (is_operator(current_token->type))
			{
				handle_operator(current_token, &operator_stack, &lex->rpn_list[i]);
			}
			current_token = current_token->next;
		}
		pop_remaining_operators(&operator_stack, &lex->rpn_list[i]);
	}
	free(operator_stack.data);
}

void	rpn(t_lex *lex)
{
	shunting_yard(lex);
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