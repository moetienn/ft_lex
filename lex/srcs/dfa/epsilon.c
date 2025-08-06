#include "../../includes/lex.h"
#include "../../includes/dfa/dfa.h"

void	epsilon_closure(t_nfa_state *start, t_nfa_state **closure, int *closure_size, bool *visited)
{
	if (!start || visited[start->id])
		return;
	visited[start->id] = true;
	closure[(*closure_size)++] = start;
	// printf("Visiting state: %p, ID: %d\n", start, start->id);
	for (size_t i = 0; i < start->transition_count; ++i)
	{
		t_nfa_transition *tr = start->transitions[i];
		// printf("Transition symbol: %c, To state: %p\n", tr->symbol, tr->to);
		if (tr->symbol == '\0' && tr->to)
			epsilon_closure(tr->to, closure, closure_size, visited);
	}
}

void	calculate_epsilon_closure(t_nfa_state **reachable_states, int reachable_count, t_nfa_state ***closure, int *closure_size, t_memory_stack *mem_stack)
{
	*closure = malloc(sizeof(t_nfa_state*) * 100);
	CHECK_ALLOC(*closure, , "Error: Failed to allocate memory for closure states", );
	push_memory_stack(mem_stack, *closure);

	bool *closure_visited = calloc(g_nfa_state_id, sizeof(bool));
	CHECK_ALLOC(closure_visited, free(*closure), "Error: Failed to allocate memory for closure visited states", );
	push_memory_stack(mem_stack, closure_visited);

	*closure_size = 0;
	// printf("Calculating epsilon closure for reachable states\n");
	for (int i = 0; i < reachable_count; ++i)
	{
		// printf("Processing reachable state: %p\n", reachable_states[i]);
		epsilon_closure(reachable_states[i], *closure, closure_size, closure_visited);
	}
}

void    compute_closure_epsilon(t_lex *lex, t_memory_stack *mem_stack)
{
	int         max_states = g_nfa_state_id;
	t_nfa_state **closure = malloc(max_states * sizeof(t_nfa_state *));
	bool        *visited = calloc(max_states, sizeof(bool));
	int         closure_size = 0;

	push_memory_stack(mem_stack, closure);
	// printf("Computing closure epsilon for super start state: %p\n", lex->super_start);
	// printf("Max states: %d\n", max_states);
	epsilon_closure(lex->super_start, closure, &closure_size, visited);
	// printf("Closure size: %d\n", closure_size);
	t_dfa_state *start_state = malloc(sizeof(t_dfa_state));
	if (!start_state)
	{
		fprintf(stderr, "Error: Failed to allocate memory for DFA start state\n");
		return;
	}
	push_memory_stack(mem_stack, start_state);
	start_state->id = g_dfa_state_id++;
	start_state->nfa_states = closure;
	start_state->action_id = 0;
	start_state->nfa_state_count = closure_size;
	start_state->transitions = NULL;
	start_state->transition_count = 0;
	start_state->is_accept = false;
	lex->dfa = malloc(sizeof(t_dfa));
	lex->dfa->start_state = start_state;
	free(visited);
}