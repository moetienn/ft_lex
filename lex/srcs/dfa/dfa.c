#include "../../includes/lex.h"

#undef CHECK_ALLOC
#define CHECK_ALLOC(ptr, cleanup, msg, ret_val) \
    if (!(ptr)) { \
        fprintf(stderr, msg "\n"); \
        cleanup; \
        return ret_val; \
    }

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

void calculate_epsilon_closure(t_nfa_state **reachable_states, int reachable_count, t_nfa_state ***closure, int *closure_size)
{
    *closure = malloc(sizeof(t_nfa_state*) * 100);
    CHECK_ALLOC(*closure, , "Error: Failed to allocate memory for closure states", );

    bool *closure_visited = calloc(g_nfa_state_id, sizeof(bool));
    CHECK_ALLOC(closure_visited, free(*closure), "Error: Failed to allocate memory for closure visited states", );

    *closure_size = 0;
    for (int i = 0; i < reachable_count; ++i)
        epsilon_closure(reachable_states[i], *closure, closure_size, closure_visited);
    free(closure_visited);
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
	// free(closure);
	free(visited);
}

void	find_reachable_states(t_dfa_state *current_state, char symbol, t_nfa_state ***reachable_states, int *reachable_count)
{
    *reachable_states = malloc(sizeof(t_nfa_state*) * 100); // Initial capacity
    if (!*reachable_states)
    {
        fprintf(stderr, "Error: Failed to allocate memory for reachable states\n");
        return;
    }
    bool *visited = calloc(g_nfa_state_id, sizeof(bool));
    CHECK_ALLOC(visited, free(*reachable_states), "Error: Failed to allocate memory for visited states", );
    *reachable_count = 0;
    for (int i = 0; i < current_state->nfa_state_count; ++i)
    {
        t_nfa_state *nfa_state = current_state->nfa_states[i];
        for (size_t j = 0; j < nfa_state->transition_count; ++j)
        {
            t_nfa_transition *tr = nfa_state->transitions[j];
            if (tr->symbol == symbol && tr->to && !visited[tr->to->id])
            {
                visited[tr->to->id] = true;
                (*reachable_states)[(*reachable_count)++] = tr->to;
            }
        }
    }
    free(visited);
}

static t_dfa_state *find_or_create_next_state(t_nfa_state **closure, int closure_size, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist)
{
    t_dfa_state *next_state = NULL;
    for (int i = 0; i < *dfa_state_count; ++i)
    {
        if ((*dfa_states)[i]->nfa_state_count == closure_size)
        {
            bool found = true;
            for (int j = 0; j < closure_size; ++j)
            {
                if ((*dfa_states)[i]->nfa_states[j] != closure[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return ((*dfa_states)[i]);
        }
    }

    next_state = malloc(sizeof(t_dfa_state));
    CHECK_ALLOC(next_state, free(closure), "Error: Failed to allocate memory for next DFA state", NULL);
    next_state->id = g_dfa_state_id++;
    next_state->nfa_states = closure;
    next_state->nfa_state_count = closure_size;
    next_state->transitions = NULL;
    next_state->transition_count = 0;

    if (*dfa_state_count % 100 == 0)
    {
        *dfa_states = realloc(*dfa_states, sizeof(t_dfa_state*) * (*dfa_state_count + 100));
        CHECK_ALLOC(*dfa_states, free(next_state), "Error: Failed to reallocate memory for DFA states", NULL);
    }
    (*dfa_states)[(*dfa_state_count)++] = next_state;
    worklist_push(worklist, next_state);

    return (next_state);
}

static void add_dfa_transition(t_dfa_state *current_state, char symbol, t_dfa_state *next_state)
{
    current_state->transitions = realloc(current_state->transitions, sizeof(t_dfa_transition*) * (current_state->transition_count + 1));
    CHECK_ALLOC(current_state->transitions, , "Error: Failed to reallocate memory for DFA transitions", );
    current_state->transitions[current_state->transition_count] = malloc(sizeof(t_dfa_transition));
    CHECK_ALLOC(current_state->transitions[current_state->transition_count], , "Error: Failed to allocate memory for DFA transition", );
    current_state->transitions[current_state->transition_count]->symbol = symbol;
    current_state->transitions[current_state->transition_count]->next_state = next_state;
    current_state->transition_count++;
    printf("Added transition: %d --%c--> %d\n", current_state->id, symbol, next_state->id);
}

void process_symbol(t_dfa_state *current_state, char symbol, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist, int alphabet_size)
{
    (void)alphabet_size; // Unused parameter
    t_nfa_state **reachable_states = NULL;
    int reachable_count = 0;
    find_reachable_states(current_state, symbol, &reachable_states, &reachable_count);
    if (reachable_count == 0)
        return;

    t_nfa_state **closure = NULL;
    int closure_size = 0;
    calculate_epsilon_closure(reachable_states, reachable_count, &closure, &closure_size);
    free(reachable_states);

    t_dfa_state *next_state = find_or_create_next_state(closure, closure_size, dfa_states, dfa_state_count, worklist);
    add_dfa_transition(current_state, symbol, next_state);
}

void	from_nfa_to_dfa(t_lex *lex)
{
	if (!lex || !lex->super_start)
	{
		fprintf(stderr, "Error: lex or super_start is NULL\n");
		return;
	}
	char	alphabet[256];
	int		alphabet_size = 0;
	collect_alphabet_from_nfa(lex->super_start, alphabet, &alphabet_size, 256);

	printf("ALPHABET: ");
	for (int i = 0; i < alphabet_size; ++i)
    	printf("[%c] ", alphabet[i]);
	printf("\n");

	compute_closure_epsilon(lex);

	t_worklist	worklist;
	init_worklist(&worklist);
	worklist_push(&worklist, lex->dfa->start_state);

	t_dfa_state	**dfa_states = malloc(sizeof(t_dfa_state*) * 100);
	if (!dfa_states)
	{
		fprintf(stderr, "Error: Failed to allocate memory for DFA states\n");
		return;
	}

	int dfa_state_count = 0;
	dfa_states[dfa_state_count++] = lex->dfa->start_state;

	while (worklist.size > 0)
	{
		t_dfa_state *current_state = worklist_pop(&worklist);
		if (!current_state)
			continue;
		for (int a = 0; a < alphabet_size; ++a)
		{
			char symbol = alphabet[a];
			process_symbol(current_state, symbol, &dfa_states, &dfa_state_count, &worklist, alphabet_size);
		}
	}
	free(lex->dfa->start_state);
	free(lex->dfa);
}
