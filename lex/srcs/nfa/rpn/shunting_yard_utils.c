#include "../../../includes/lex.h"
#include "../../../includes/nfa/nfa.h"

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
