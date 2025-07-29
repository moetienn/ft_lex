#include "../../includes/dfa/dfa.h"

int	add_to_alphabet(char *alphabet, int *alphabet_size, char symbol)
{
    for (int i = 0; i < *alphabet_size; ++i)
        if (alphabet[i] == symbol)
            return 0;
    alphabet[(*alphabet_size)++] = symbol;
    return 1;
}

// DFS pour visiter tous les états atteignables et collecter l'alphabet
void	collect_alphabet_dfs(t_nfa_state *state, char *alphabet, int *alphabet_size, bool *visited, int max_states)
{
    if (!state || visited[state->id])
        return;
    visited[state->id] = true;
    for (size_t i = 0; i < state->transition_count; ++i) {
        t_nfa_transition *tr = state->transitions[i];
        if (tr->symbol != '\0')
            add_to_alphabet(alphabet, alphabet_size, tr->symbol);
        if (tr->to)
            collect_alphabet_dfs(tr->to, alphabet, alphabet_size, visited, max_states);
    }
}

void	collect_alphabet_from_nfa(t_nfa_state *super_start, char *alphabet, int *alphabet_size, int max_state_id)
{
    bool *visited = calloc(max_state_id + 1, sizeof(bool));
    *alphabet_size = 0;
    collect_alphabet_dfs(super_start, alphabet, alphabet_size, visited, max_state_id + 1);
    free(visited);
}