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
    *reachable_states = malloc(sizeof(t_nfa_state*) * 100);
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

static t_dfa_state *find_or_create_next_state(t_nfa_state **closure, int closure_size, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist, t_dfa *dfa)
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
    dfa->state_count = *dfa_state_count;
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

void process_symbol(t_dfa_state *current_state, char symbol, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist, int alphabet_size, t_dfa *dfa)
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

    t_dfa_state *next_state = find_or_create_next_state(closure, closure_size, dfa_states, dfa_state_count, worklist, dfa);
    add_dfa_transition(current_state, symbol, next_state);
}

// void	generate_lexyyc(t_lex *lex)
// {
// 	if (!lex || !lex->dfa)
// 	{
// 		fprintf(stderr, "Error: lex or dfa is NULL\n");
// 		return;
// 	}

// 	printf("Generating lex.yy.c...\n");
// 	FILE *file = fopen("lex.yy.c", "w");
// 	if (!file)
// 	{
// 		perror("Failed to open lex.yyc.c for writing");
// 		return;
// 	}

// 	fprintf(file, "#include \"libl.h\"\n");
// 	fprintf(file, lex->declaration_code, "lexyyc");
// 	fprintf(file, "\n\n");
// 	fprintf(file, "int yylex(void) {\n");
// 	fprintf(file, "}\n");

// 	fprintf(file, "\n\n");
// 	fprintf(file, lex->user_code, "lexyyc");

// 	fclose(file);
// 	printf("lexyyc.c generated successfully.\n");
// }

void	generate_lexyyc(t_lex *lex, const char *alphabet, int alphabet_size)
{
	if (!lex || !lex->dfa)
	{
		fprintf(stderr, "Error: lex or dfa is NULL\n");
		return;
	}

	printf("Generating lex.yy.c...\n");
	FILE *file = fopen("lex.yy.c", "w");
	if (!file)
	{
		perror("Failed to open lex.yy.c for writing");
		return;
	}

	fprintf(file, "#include \"libl/includes/libl.h\"\n");
	if (lex->declaration_code)
		fprintf(file, "%s\n", lex->declaration_code);

    printf("declaration code OK \n");

	fprintf(file, "const char DFA_ALPHABET[] = {");
	for (int i = 0; i < alphabet_size; ++i) {
		if (i > 0) fprintf(file, ", ");
		if (alphabet[i] >= 32 && alphabet[i] < 127)
			fprintf(file, "'%c'", alphabet[i]);
		else
			fprintf(file, "%d", (unsigned char)alphabet[i]);
	}
	fprintf(file, "};\n#define DFA_ALPHABET_SIZE %d\n\n", alphabet_size);

    printf("alphabet OK \n");

	fprintf(file,
		"typedef struct {\n"
		"    int is_accept;\n"
		"    int transitions[DFA_ALPHABET_SIZE];\n"
		"    int action_id; // 0 = pas d'action, n > 0 = numéro d'action\n"
		"} DFA_State;\n\n");

    printf("struct definition OK \n");

	fprintf(file, "DFA_State DFA_STATES[] = {\n");
    printf("lex->dfa->state_count: %d\n", lex->dfa->state_count);
	for (int s = 0; s < lex->dfa->state_count; ++s)
    {
        printf("lex->dfa->states[%d]: %p\n", s, lex->dfa->states[s]);
        t_dfa_state *state = lex->dfa->states[s];
		fprintf(file, "    { %d, {", state->is_accept ? 1 : 0);
		for (int a = 0; a < alphabet_size; ++a)
        {
			char sym = alphabet[a];
			int dest = -1;
            printf("Processing symbol: %c\n", sym);
			for (int t = 0; t < state->transition_count; ++t)
            {
				if (state->transitions[t]->symbol == sym)
					dest = state->transitions[t]->next_state->id;
			}
			fprintf(file, "%d", dest);
			if (a < alphabet_size - 1)
				fprintf(file, ", ");
		}
		fprintf(file, "}, %d }", 1);
		if (s < lex->dfa->state_count - 1)
			fprintf(file, ",\n");
		else
			fprintf(file, "\n");
	}
	fprintf(file, "};\n#define DFA_STATE_COUNT %d\n\n", lex->dfa->state_count);

    printf("DFA states OK \n");
	fprintf(file,
		"int dfa_symbol_index(char c) {\n"
		"    for (int i = 0; i < DFA_ALPHABET_SIZE; ++i)\n"
		"        if (DFA_ALPHABET[i] == c) return i;\n"
		"    return -1;\n"
		"}\n\n");

	fprintf(file, "void yy_action(int action_id) {\n");
	fprintf(file, "    switch(action_id) {\n");
	for (size_t i = 0; i < lex->rules_list.count; ++i) {
		fprintf(file, "        case %ld: %s; break;\n", i+1, lex->rules_list.list[i].action);
	}
    for (size_t i = 0; i < lex->rules_list.count; ++i) {
		printf("        case %ld: %s; break;\n", i+1, lex->rules_list.list[i].action);
	}
	fprintf(file, "    }\n}\n\n");

	fprintf(file,
		"int yylex(void) {\n"
		"    int c, idx, state, last_accept, last_accept_pos, i;\n"
		"    char buffer[4096];\n"
		"    int bufpos = 0, input_char;\n\n"
		"    while (1) {\n"
		"        bufpos = 0;\n"
		"        state = 0;\n"
		"        last_accept = -1;\n"
		"        last_accept_pos = -1;\n\n"
		"        while ((input_char = input()) > 0) {\n"
		"            c = input_char;\n"
		"            idx = dfa_symbol_index(c);\n"
		"            if (idx == -1) break;\n"
		"            buffer[bufpos++] = c;\n"
		"            state = DFA_STATES[state].transitions[idx];\n"
		"            if (state == -1) break;\n"
		"            if (DFA_STATES[state].is_accept) {\n"
		"                last_accept = state;\n"
		"                last_accept_pos = bufpos;\n"
		"            }\n"
		"        }\n"
		"        if (last_accept != -1) {\n"
		"            buffer[last_accept_pos] = '\\0';\n"
		"            yytext = buffer;\n"
		"            yyleng = last_accept_pos;\n"
		"            yy_action(DFA_STATES[last_accept].action_id);\n"
		"            for (i = bufpos-1; i >= last_accept_pos; --i)\n"
		"                unput(buffer[i]);\n"
		"        } else {\n"
		"            if (input_char <= 0)\n"
		"                break;\n"
		"        }\n"
		"    }\n"
		"    return 0;\n"
		"}\n\n"
	);

	if (lex->user_code)
		fprintf(file, "%s\n", lex->user_code);

	fclose(file);
	printf("lex.yy.c generated successfully.\n");
}

void from_nfa_to_dfa(t_lex *lex)
{
    if (!lex || !lex->super_start)
    {
        fprintf(stderr, "Error: lex or super_start is NULL\n");
        return;
    }
    char alphabet[256];
    int alphabet_size = 0;
    collect_alphabet_from_nfa(lex->super_start, alphabet, &alphabet_size, 256);

    printf("ALPHABET: ");
    for (int i = 0; i < alphabet_size; ++i)
        printf("[%c] ", alphabet[i]);
    printf("\n");

    compute_closure_epsilon(lex);

    t_worklist worklist;
    init_worklist(&worklist);
    worklist_push(&worklist, lex->dfa->start_state);

    t_dfa_state **dfa_states = malloc(sizeof(t_dfa_state*) * 100);
    if (!dfa_states)
    {
        fprintf(stderr, "Error: Failed to allocate memory for DFA states\n");
        return;
    }

    int dfa_state_count = 0;
    dfa_states[dfa_state_count++] = lex->dfa->start_state;
    lex->dfa->state_count = dfa_state_count;

    while (worklist.size > 0)
    {
        t_dfa_state *current_state = worklist_pop(&worklist);
        if (!current_state)
            continue;
        for (int a = 0; a < alphabet_size; ++a)
        {
            char symbol = alphabet[a];
            process_symbol(current_state, symbol, &dfa_states, &dfa_state_count, &worklist, alphabet_size, lex->dfa);
        }
    }
    lex->dfa->states = dfa_states;
    lex->dfa->accept_states = malloc(sizeof(t_dfa_state*) * dfa_state_count);


    generate_lexyyc(lex, alphabet, alphabet_size);

    free(lex->dfa->start_state);
    free(lex->dfa);
}
