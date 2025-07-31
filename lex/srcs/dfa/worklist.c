#include "../../includes/dfa/dfa.h"

void	free_worklist(t_worklist *wl)
{
	if (!wl)
		return;
	for (int i = 0; i < wl->size; ++i)
	{
		free(wl->states[i]->transitions);
		free(wl->states[i]->nfa_states);
		free(wl->states[i]);
	}
	free(wl->states);
	wl->states = NULL;
	wl->size = 0;
	wl->capacity = 0;
}

void	init_worklist(t_worklist *wl)
{
	wl->size = 0;
	wl->capacity = 8;
	wl->states = malloc(wl->capacity * sizeof(t_dfa_state *));
	if (!wl->states)
	{
		fprintf(stderr, "Error: Failed to allocate memory for worklist states\n");
		exit(EXIT_FAILURE);
	}
}

void	worklist_push(t_worklist *wl, t_dfa_state *state)
{
	if (wl->size == wl->capacity)
	{
		wl->capacity *= 2;
		wl->states = realloc(wl->states, wl->capacity * sizeof(t_dfa_state *));
	}
	wl->states[wl->size++] = state;
}

t_dfa_state	*worklist_pop(t_worklist *wl)
{
	if (wl->size == 0) return NULL;
	return wl->states[--wl->size];
}