#include "../../../includes/lex.h"

char decode_escape(char c)
{
	switch (c)
	{
		case 'n': return '\n';
		case 't': return '\t';
		case 'r': return '\r';
		case 'b': return '\b';
		case 'f': return '\f';
		case '\\': return '\\';
		case '\'': return '\'';
		case '"': return '"';
		default: return c;
	}
}

t_nfa_fragment	*process_token_escape(t_token *current_token)
{
	t_nfa_state *start = init_nfa_state(-1, false);
	t_nfa_state *accept = init_nfa_state(-1, false);
	t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
	if (!transition)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	transition->to = accept;
	transition->symbol = decode_escape(current_token->value[1]);
	add_transition(start, transition);
	t_nfa_fragment *fragment = init_nfa_fragment(start, accept);
	return fragment;
}

t_nfa_fragment	*process_token_char(t_token *current_token)
{
	t_nfa_state *start = init_nfa_state(-1, false);
	t_nfa_state *accept = init_nfa_state(-1, false);
	t_nfa_transition *transition = malloc(sizeof(t_nfa_transition));
	if (!transition)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	transition->to = accept;
	transition->symbol = current_token->value[0];
	add_transition(start, transition);
	t_nfa_fragment *fragment = init_nfa_fragment(start, accept);
	return fragment;
}
