#include "../../includes/lex.h"

int g_dfa_state_id = 0;

static void add_dfa_transition(t_dfa_state *current_state, char symbol, t_dfa_state *next_state)
{
	current_state->transitions = realloc(current_state->transitions, sizeof(t_dfa_transition*) * (current_state->transition_count + 1));
	CHECK_ALLOC(current_state->transitions, , "Error: Failed to reallocate memory for DFA transitions", );
	current_state->transitions[current_state->transition_count] = malloc(sizeof(t_dfa_transition));
	CHECK_ALLOC(current_state->transitions[current_state->transition_count], , "Error: Failed to allocate memory for DFA transition", );
	current_state->transitions[current_state->transition_count]->symbol = symbol;
	current_state->transitions[current_state->transition_count]->next_state = next_state;
	current_state->transition_count++;
}

void	process_symbol(t_dfa_state *current_state, char symbol, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist, t_dfa *dfa)
{
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
			process_symbol(current_state, symbol, &dfa_states, &dfa_state_count, &worklist, lex->dfa);
		}
	}
	lex->dfa->states = dfa_states;
	lex->dfa->accept_states = malloc(sizeof(t_dfa_state*) * dfa_state_count);
	int accept_count = 0;
	for (int i = 0; i < dfa_state_count; ++i)
	{
		t_dfa_state *state = dfa_states[i];
		for (int j = 0; j < state->nfa_state_count; ++j)
		{
			if (state->nfa_states[j]->is_accept)
			{
				state->is_accept = 1;
				state->action_id = state->nfa_states[j]->rule_index;
				state->action_id += 1;
				printf("Accept state found: ID %d, Action ID %d\n", state->id, state->action_id);
				lex->dfa->accept_states[accept_count++] = state;
				break;
			}
		}
	}
	generate_lexyyc(lex, alphabet, alphabet_size);
	free(lex->dfa->start_state);
	free(lex->dfa);
}
