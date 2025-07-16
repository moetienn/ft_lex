#include "../../../includes/lex.h"
#include "../../../includes/nfa/nfa.h"

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
		return true;
	return false;
}

bool is_operand(t_token_type type)
{
	return (type == TOKEN_CHAR || type == TOKEN_CLASS || type == TOKEN_QUOTE ||
			type == TOKEN_ESCAPE);
}

bool is_operator(t_token_type type)
{
	return (type == TOKEN_CONCAT || type == TOKEN_ALTERNATION ||
			type == TOKEN_KLEENE_STAR || type == TOKEN_PLUS ||
			type == TOKEN_OPTIONAL || type == TOKEN_QUANTIFIER ||
			type == TOKEN_LPAREN || type == TOKEN_RPAREN);
}
