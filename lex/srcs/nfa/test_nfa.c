#include "../../includes/lex.h"
#include "../../includes/nfa/nfa.h"

typedef struct {
    t_nfa_state **states;
    size_t count;
    size_t capacity;
} state_set;

void add_state(state_set *set, t_nfa_state *state)
{
    for (size_t i = 0; i < set->count; i++)
        if (set->states[i] == state)
            return;
    if (set->count == set->capacity)
    {
        set->capacity = set->capacity ? set->capacity * 2 : 8;
        set->states = realloc(set->states, set->capacity * sizeof(t_nfa_state *));
        if (!set->states)
        {
            perror("Out of memory");
            exit(1);
        }
    }
    set->states[set->count++] = state;
}

void free_state_set(state_set *set)
{
    free(set->states);
    set->states = NULL;
    set->count = set->capacity = 0;
}

void epsilon_closure(state_set *set, t_nfa_state *state)
{
    add_state(set, state);
    for (size_t i = 0; i < state->transition_count; i++)
    {
        t_nfa_transition *t = state->transitions[i];
        if (t->symbol == '\0')
        {
            bool already = false;
            for (size_t j = 0; j < set->count; j++)
                if (set->states[j] == t->to)
                    already = true;
            if (!already)
                epsilon_closure(set, t->to);
        }
    }
}

bool test_nfa(t_nfa_state *start_state, const char *input)
{
    state_set current = {0}, next = {0};
    epsilon_closure(&current, start_state);

    const char *ptr = input;
    printf("=== NFA simulation for input: \"%s\" ===\n", input);

    while (*ptr)
    {
        next.count = 0;

        for (size_t i = 0; i < current.count; i++)
        {
            t_nfa_state *state = current.states[i];
            // printf("At state id=%d, processing input '%c'\n", state->id, *ptr);
            for (size_t t = 0; t < state->transition_count; t++)
            {
                t_nfa_transition *trans = state->transitions[t];
                // printf("  Checking transition: symbol='%c' (to state id=%d)\n",
                //     trans->symbol ? trans->symbol : '\0', trans->to->id);
                if (trans->symbol == *ptr)
                {
                    // printf("  >> Taking normal transition to state id=%d\n", trans->to->id);
                    epsilon_closure(&next, trans->to);
                }
            }
        }
        if (next.count == 0)
        {
            free_state_set(&current);
            free_state_set(&next);
            return false;
        }
        state_set tmp = current;
        current = next;
        next = tmp;
        ptr++;
    }
    next.count = 0;
    for (size_t i = 0; i < current.count; ++i)
        epsilon_closure(&next, current.states[i]);
    bool accepted = false;
    for (size_t i = 0; i < next.count; i++)
    {
        if (next.states[i]->is_accept)
        {
            accepted = true;
            break;
        }
    }
    // printf("End of input. Accepting? %s\n", accepted ? "YES" : "NO");
    free_state_set(&current);
    free_state_set(&next);
    return accepted;
}