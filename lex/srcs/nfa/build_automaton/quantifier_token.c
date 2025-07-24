#include "../../../includes/lex.h"

t_nfa_fragment    *clone_fragment(t_nfa_fragment *fragment)
{
	if (!fragment)
		return NULL;

	t_nfa_state *new_start = init_nfa_state(fragment->start->id, false);
	t_nfa_state *new_accept = init_nfa_state(fragment->accept->id, fragment->accept->is_accept);

	t_nfa_fragment *new_fragment = init_nfa_fragment(new_start, new_accept);

	for (size_t i = 0; i < fragment->start->transition_count; i++)
	{
		t_nfa_transition *transition = fragment->start->transitions[i];
		t_nfa_transition *new_transition = create_transition(new_accept, transition->symbol);
		add_transition(new_start, new_transition);
	}
	return (new_fragment);
}

static int parse_quantifier(const char *value, int *min_repetitions, int *max_repetitions)
{
    char *comma_pos = strchr(value, ',');
    if (value[0] != '{' || value[strlen(value) - 1] != '}')
	{
        fprintf(stderr, "Invalid quantifier format: %s\n", value);
        return -1;
    }
    *min_repetitions = atoi(value + 1);
    *max_repetitions = *min_repetitions;
    if (comma_pos)
        *max_repetitions = atoi(comma_pos + 1);
    if (*min_repetitions < 0 || *max_repetitions < *min_repetitions)
	{
        fprintf(stderr, "Invalid quantifier range: %s\n", value);
        return -1;
    }
    return (0);
}

static void add_first_transition(t_nfa_state *current_accept, t_nfa_fragment *original_fragment)
{
    t_nfa_transition *first_transition = create_transition(original_fragment->start, '\0');
    if (!first_transition)
	{
        fprintf(stderr, "Error: Failed to create first transition\n");
        exit(EXIT_FAILURE);
    }
    add_transition(current_accept, first_transition);
}

static void add_mandatory_repetitions(t_nfa_state **current_accept, t_nfa_fragment *original_fragment, int min_repetitions)
{
    for (int i = 1; i < min_repetitions; i++)
	{
        t_nfa_fragment *cloned_fragment = clone_fragment(original_fragment);
        if (!cloned_fragment)
		{
            fprintf(stderr, "Error: Failed to clone fragment\n");
            exit(EXIT_FAILURE);
        }
        t_nfa_transition *transition = create_transition(cloned_fragment->start, '\0');
        if (!transition)
		{
            fprintf(stderr, "Error: Failed to create transition\n");
            exit(EXIT_FAILURE);
        }
        add_transition(*current_accept, transition);
        *current_accept = cloned_fragment->accept;
        free(cloned_fragment);
    }
}

static void add_optional_repetitions(t_nfa_state **current_accept, t_nfa_fragment *original_fragment, int min_repetitions, int max_repetitions)
{
    for (int i = min_repetitions; i < max_repetitions; i++)
	{
        t_nfa_fragment *cloned_fragment = clone_fragment(original_fragment);
        if (!cloned_fragment)
		{
            fprintf(stderr, "Error: Failed to clone fragment\n");
            exit(EXIT_FAILURE);
        }
        t_nfa_transition *transition = create_transition(cloned_fragment->start, '\0');
        if (!transition)
		{
            fprintf(stderr, "Error: Failed to create transition\n");
            exit(EXIT_FAILURE);
        }
        add_transition(*current_accept, transition);
        t_nfa_transition *epsilon_transition = create_transition(cloned_fragment->accept, '\0');
        if (!epsilon_transition)
		{
            fprintf(stderr, "Error: Failed to create epsilon transition\n");
            exit(EXIT_FAILURE);
        }
        add_transition(*current_accept, epsilon_transition);
        *current_accept = cloned_fragment->accept;
        free(cloned_fragment);
    }
}

static t_nfa_fragment *finalize_quantified_fragment(t_nfa_state *new_start, t_nfa_state *current_accept)
{
    current_accept->is_accept = true;
    t_nfa_fragment *quantified_fragment = init_nfa_fragment(new_start, current_accept);

    if (!quantified_fragment)
	{
        fprintf(stderr, "Error: Failed to create quantified fragment\n");
        exit(EXIT_FAILURE);
    }
    return (quantified_fragment);
}

void    process_token_quantifier(t_frag_stack *frag_stack, t_token *current_token)
{
    t_nfa_fragment *original_fragment = pop_stack_frag(frag_stack);

    if (!original_fragment)
	{
        fprintf(stderr, "Error: Original fragment is NULL\n");
        exit(EXIT_FAILURE);
    }
    int min_repetitions, max_repetitions;
    if (parse_quantifier(current_token->value, &min_repetitions, &max_repetitions) != 0)
        exit(EXIT_FAILURE);
    t_nfa_state *new_start = init_nfa_state(-1, false);
    t_nfa_state *current_accept = new_start;
    add_first_transition(current_accept, original_fragment);
    current_accept = original_fragment->accept;
    add_mandatory_repetitions(&current_accept, original_fragment, min_repetitions);
    add_optional_repetitions(&current_accept, original_fragment, min_repetitions, max_repetitions);
    t_nfa_fragment *quantified_fragment = finalize_quantified_fragment(new_start, current_accept);
    push_stack_frag(frag_stack, quantified_fragment);
    free(original_fragment);
}
