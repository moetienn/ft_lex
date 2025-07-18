#include "../../includes/lex.h"

bool test_nfa(t_nfa_state *start_state, const char *input)
{
    t_nfa_state *current_state = start_state;
    const char *ptr = input;
    while (*ptr)
    {
        bool transition_found = false;

        for (size_t i = 0; i < current_state->transition_count; i++)
        {
            t_nfa_transition *transition = current_state->transitions[i];
            if (transition->symbol == *ptr || transition->symbol == '\0')
            {
                current_state = transition->to;
                transition_found = true;
                break;
            }
        }
        if (!transition_found)
            return (false);
        ptr++;
    }
    return (current_state->is_accept);
}