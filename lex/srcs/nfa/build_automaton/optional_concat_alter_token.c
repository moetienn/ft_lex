#include "../../../includes/lex.h"

void    process_token_optional(t_frag_stack *frag_stack, int rule_index)
{
	t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
	t_nfa_state *new_start = init_nfa_state(-1, false, rule_index);
	t_nfa_state *new_accept = init_nfa_state(-1, false, rule_index);


	add_enter_transition(new_start, fragment->start);
	add_exit_transition(fragment->accept, new_accept);
	add_empty_transition(new_start, new_accept);

	t_nfa_fragment *optional_fragment = init_nfa_fragment(new_start, new_accept);
	push_stack_frag(frag_stack, optional_fragment);
	free(fragment);
}

void	process_token_alternation(t_frag_stack *frag_stack, int rule_index)
{
	t_nfa_fragment *fragment_b = pop_stack_frag(frag_stack);
	t_nfa_fragment *fragment_a = pop_stack_frag(frag_stack);

	t_nfa_state *new_start = init_nfa_state(-1, false, rule_index);
	t_nfa_state *new_accept = init_nfa_state(-1, false, rule_index);
	add_enter_transition(new_start, fragment_a->start);
	add_enter_transition(new_start, fragment_b->start);
	add_exit_transition(fragment_a->accept, new_accept);
	add_exit_transition(fragment_b->accept, new_accept);
	t_nfa_fragment *alt_fragment = init_nfa_fragment(new_start, new_accept);
	push_stack_frag(frag_stack, alt_fragment);
	free(fragment_a);
	free(fragment_b);
}

void	process_token_concat(t_frag_stack *frag_stack)
{
	t_nfa_fragment *right = pop_stack_frag(frag_stack);
	t_nfa_fragment *left = pop_stack_frag(frag_stack);


	t_nfa_transition *epsilon = malloc(sizeof(t_nfa_transition));
	if (!epsilon)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	epsilon->to = right->start;
	epsilon->symbol = '\0';
	add_transition(left->accept, epsilon);

	left->accept->is_accept = false;
	t_nfa_fragment *concat_fragment = init_nfa_fragment(left->start, right->accept);
	push_stack_frag(frag_stack, concat_fragment);

	free(left);
	free(right);
}
