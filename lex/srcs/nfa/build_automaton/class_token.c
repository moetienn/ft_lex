#include "../../../includes/lex.h"

t_nfa_fragment	*process_token_class(t_token *current_token, int rule_index)
{
	t_nfa_state *start = init_nfa_state(-1, false, rule_index);
	t_nfa_state *accept = init_nfa_state(-1, true, rule_index);
	t_nfa_fragment *fragment = init_nfa_fragment(start, accept);

	const char *value = current_token->value;
	if (value[0] != '[' || value[strlen(value) - 1] != ']')
	{
		fprintf(stderr, "Invalid character class format: %s\n", value);
		exit(EXIT_FAILURE);
	}
	for (size_t i = 1; i < strlen(value) - 1; i++)
	{
		if (value[i + 1] == '-' && i + 2 < strlen(value) - 1)
		{
			char start_char = value[i];
			char end_char = value[i + 2];
			for (char c = start_char; c <= end_char; c++)
			{
				t_nfa_transition *transition = create_transition(accept, c);
				add_transition(start, transition);
			}
			i += 2;
		}
		else
		{
			t_nfa_transition *transition = create_transition(accept, value[i]);
			add_transition(start, transition);
		}
	}
	return (fragment);
}
