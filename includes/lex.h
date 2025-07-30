typedef struct s_nfa_transition {
    int symbol;
    struct s_nfa_state *next_state;
} t_nfa_transition;

typedef struct s_nfa_state {
    int id;
    t_nfa_transition **transitions;
    int transition_count;
    int is_accept;
    int action_id; // Added field to store the action ID for NFA states
} t_nfa_state;

typedef struct s_dfa_transition {
    int input_symbol;
    struct s_dfa_state *next_state;
} t_dfa_transition;

typedef struct s_dfa_state {
    int id;
    t_nfa_state **nfa_states;
    int nfa_state_count;
    t_dfa_transition **transitions;
    int transition_count;
    int is_accept;
    int action_id; // Added field to store the action ID
} t_dfa_state;