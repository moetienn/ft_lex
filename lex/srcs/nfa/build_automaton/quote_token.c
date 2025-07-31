#include "../../../includes/lex.h"

t_nfa_fragment  *process_token_quote(t_token *current_token, int rule_index)
{
    if (current_token->value[0] != '"' || current_token->value[strlen(current_token->value) - 1] != '"')
    {
        fprintf(stderr, "Invalid quote token format: %s\n", current_token->value);
        exit(EXIT_FAILURE);
    }

    t_nfa_state *start = init_nfa_state(-1, false, rule_index);
    t_nfa_state *accept = init_nfa_state(-1, true, rule_index);
    t_nfa_fragment *fragment = init_nfa_fragment(start, accept);

    for (size_t i = 1; i < strlen(current_token->value) - 1; i++)
    {
        t_nfa_transition *transition = create_transition(accept, current_token->value[i]);
        add_transition(start, transition);
    }

    return (fragment);
}