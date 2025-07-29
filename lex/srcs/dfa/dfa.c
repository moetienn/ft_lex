#include "../../includes/lex.h"

void	from_nfa_to_dfa(t_lex *lex)
{
    if (!lex || !lex->super_start) {
        fprintf(stderr, "Error: lex or super_start is NULL\n");
        return;
    }
	printf("Converting NFA to DFA...\n");
}