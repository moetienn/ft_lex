#include "../../includes/lex.h"
#include "../../includes/dfa/dfa.h"

void	find_reachable_states(t_dfa_state *current_state, char symbol, t_nfa_state ***reachable_states, int *reachable_count, t_memory_stack *mem_stack)
{
	*reachable_states = malloc(sizeof(t_nfa_state*) * 100);
	if (!*reachable_states)
	{
		fprintf(stderr, "Error: Failed to allocate memory for reachable states\n");
		return;
	}
	push_memory_stack(mem_stack, *reachable_states);
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

t_dfa_state *find_or_create_next_state(t_nfa_state **closure, int closure_size, t_dfa_state ***dfa_states, int *dfa_state_count, t_worklist *worklist, t_dfa *dfa)
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
