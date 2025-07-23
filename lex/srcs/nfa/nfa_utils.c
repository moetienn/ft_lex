#include "../../includes/lex.h"

void	free_all_rules_frags(t_nfa_fragment **rule_frags, size_t rule_count)
{
	if (!rule_frags)
		return;
	for (size_t i = 0; i < rule_count; i++)
	{
		if (!rule_frags[i])
			continue;
		if (rule_frags[i])
			free(rule_frags[i]);
	}
	if (rule_frags)
	{
		free(rule_frags);
		rule_frags = NULL;
	}
}

void	free_nfa_state_recursive(t_nfa_state *state)
{
    if (state->visited == true)
    {
        printf("[VISITED] state %p already visited, skipping\n", (void *)state);
        return;
    }
    state->visited = true;
	if (state->visited == true)
		printf("[IS VISITED] visiting state %p\n", (void *)state);

    for (size_t i = 0; i < state->transition_count; i++)
    {
        t_nfa_transition *transition = state->transitions[i];
        if (transition->to && !transition->to->visited)
        {
			printf("[GOING IN RECURSIVE] recursively freeing state %p\n", (void *)transition->to);
            free_nfa_state_recursive(transition->to);
        }
		printf("[free_nfa_state_recursive in LOOP] freeing state->transition[i] %p\n", (void *)state->transitions[i]);
        free(transition);
    }
	printf("[free_nfa_state_recursive] freeing state->transitions %p\n", (void *)state->transitions);
    free(state->transitions);
	printf("[free_nfa_state_recursive] freeing state %p\n", (void *)state);
    free(state);
}
