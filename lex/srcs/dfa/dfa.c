#include "../../includes/lex.h"

int g_dfa_state_id = 0;

void	epsilon_closure(t_nfa_state *start, t_nfa_state **closure, int *closure_size, bool *visited)
{
    if (!start || visited[start->id])
        return;
	visited[start->id] = true;
	closure[(*closure_size)++] = start;
	for (size_t i = 0; i < start->transition_count; ++i)
	{
        t_nfa_transition *tr = start->transitions[i];
        if (tr->symbol == '\0' && tr->to)
            epsilon_closure(tr->to, closure, closure_size, visited);
    }
}

void    compute_closure_epsilon(t_lex *lex)
{
	int			max_states = g_nfa_state_id;
	t_nfa_state	**closure = malloc(max_states * sizeof(t_nfa_state *));
	bool		*visited = calloc(max_states, sizeof(bool));
	int			closure_size = 0;

	epsilon_closure(lex->super_start, closure, &closure_size, visited);
	printf("Epsilon closure have %d states\n", closure_size);
	for (int i = 0; i < closure_size; i++)
		printf("State ID: %d\n", closure[i]->id);
    t_dfa_state *start_state = malloc(sizeof(t_dfa_state));
    if (!start_state)
	{
        fprintf(stderr, "Error: Failed to allocate memory for DFA start state\n");
        return;
    }

    start_state->id = g_dfa_state_id++;
    start_state->nfa_states = closure;
    start_state->nfa_state_count = closure_size;
    start_state->transitions = NULL;
    start_state->transition_count = 0;

	lex->dfa = malloc(sizeof(t_dfa));
    lex->dfa->start_state = start_state;

	t_worklist	worklist;
	init_worklist(&worklist);
	worklist_push(&worklist, start_state);

	free(closure);
	free(visited);
}

void	from_nfa_to_dfa(t_lex *lex)
{
	if (!lex || !lex->super_start) {
		fprintf(stderr, "Error: lex or super_start is NULL\n");
		return;
	}
	printf("Converting NFA to DFA...\n");
	printf("lex->super_start ID: %d\n", lex->super_start->id);
	printf("lex->super_start->transition_count: %zu\n", lex->super_start->transition_count);

	char	alphabet[256];
	int		alphabet_size = 0;

	collect_alphabet_from_nfa(lex->super_start, alphabet, &alphabet_size, 256);

	printf("ALPHABET: ");
	for (int i = 0; i < alphabet_size; ++i)
    	printf("[%c] ", alphabet[i]);
	printf("\n");

	compute_closure_epsilon(lex);


	// Free the DFA
	free(lex->dfa->start_state);
	free(lex->dfa);
}