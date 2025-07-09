#include "../../includes/lex.h"

void	build_nfa(t_lex *lex)
{
	printf("Building NFA...\n");
	// init token structure
	init_token_list(&lex->token_list, lex->rules_list.count);
	printf("NFA start state initialized.\n");
	// 1st step: tokenize the regex patterns
	tokenize_patterns(lex);
}