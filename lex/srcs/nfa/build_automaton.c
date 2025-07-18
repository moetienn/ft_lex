#include "../../includes/lex.h"

void	add_transition(t_nfa_state *state, t_nfa_transition *transition)
{
    if (state->transition_count == state->transition_capacity)
    {
        state->transition_capacity = state->transition_capacity == 0 ? 1 : state->transition_capacity * 2;
        state->transitions = realloc(state->transitions, state->transition_capacity * sizeof(t_nfa_transition *));
        if (!state->transitions)
        {
            perror("Failed to reallocate memory for transitions");
            exit(EXIT_FAILURE);
        }
    }
    state->transitions[state->transition_count++] = transition;
}

t_nfa_state *process_token_char(t_nfa_state *current_state, t_token *current_token)
{
    t_nfa_state *new_state = init_nfa_state(-1, false);
    t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
    if (!transition)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    transition->to = new_state;
    transition->symbol = current_token->value[0];
    add_transition(current_state, transition);
    return new_state;
}
