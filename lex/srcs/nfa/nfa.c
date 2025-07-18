#include "../../includes/lex.h"

void	build_nfa_from_rpn(t_lex *lex)
{
    init_nfa(lex);
    if (!lex->rpn_list)
    {
        fprintf(stderr, "Error: RPN list is not initialized\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < lex->rules_list.count; i++)
    {
        t_token *current_token = lex->rpn_list[i];
        t_nfa_state *current_state = lex->nfa_frag->start;

        while (current_token)
        {
            if (current_token->type == TOKEN_CHAR)
                current_state = process_token_char(current_state, current_token);
            current_token = current_token->next;
		}
        current_state->is_accept = true;
    }
}

void	build_nfa(t_lex *lex)
{
	init_token_list(&lex->token_list, lex->rules_list.count);
	init_token_list(&lex->rpn_list, lex->rules_list.count);
	tokenize_patterns(lex);
	add_concat_tokens(lex);
	rpn(lex);
	build_nfa_from_rpn(lex);
	printf("Test 'ab': %s\n", test_nfa(lex->nfa_frag->start, "ab") ? "ACCEPTED" : "REJECTED");
    printf("Test 'a': %s\n", test_nfa(lex->nfa_frag->start, "a") ? "ACCEPTED" : "REJECTED");
    printf("Test 'b': %s\n", test_nfa(lex->nfa_frag->start, "b") ? "ACCEPTED" : "REJECTED");
    printf("Test 'ba': %s\n", test_nfa(lex->nfa_frag->start, "ba") ? "ACCEPTED" : "REJECTED");
    printf("Test 'abc': %s\n", test_nfa(lex->nfa_frag->start, "abc") ? "ACCEPTED" : "REJECTED");
}