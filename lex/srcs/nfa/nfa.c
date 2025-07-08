#include "../../includes/lex.h"

void	build_nfa(t_lex *lex)
{
	printf("Building NFA...\n");
	// init token structure
	init_token(&lex->nfa_start);
	printf("NFA start state initialized.\n");
	// 1st step: tokenize the regex patterns
	tokenize_patterns(lex);
}