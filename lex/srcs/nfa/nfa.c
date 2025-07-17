#include "../../includes/lex.h"

void	build_nfa_from_rpn(t_lex *lex)
{
	(void)lex; // Placeholder for future implementation
}

void	build_nfa(t_lex *lex)
{
	// init token structure
	init_token_list(&lex->token_list, lex->rules_list.count);
	init_token_list(&lex->rpn_list, lex->rules_list.count);
	// 1st step: tokenize the regex patterns
	tokenize_patterns(lex);
	add_concat_tokens(lex);
	// 2nd step: convert the tokenized patterns to rpn
	rpn(lex);
	// 3rd step: build the NFA from the RPN
	build_nfa_from_rpn(lex);
}