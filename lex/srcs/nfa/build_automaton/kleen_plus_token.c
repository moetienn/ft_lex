#include "../../../includes/lex.h"

void	process_token_kleene_star(t_frag_stack *frag_stack, int rule_index)
{
    t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
    t_nfa_state *new_start = init_nfa_state(-1, false, rule_index);
    t_nfa_state *new_accept = init_nfa_state(-1, false, rule_index);

    add_loop_transition(fragment->accept, fragment->start);
    add_exit_transition(fragment->accept, new_accept);
    add_enter_transition(new_start, fragment->start);
    add_empty_transition(new_start, new_accept);

    t_nfa_fragment *star_fragment = init_nfa_fragment(new_start, new_accept);
    push_stack_frag(frag_stack, star_fragment);
    free(fragment);
    fragment = NULL;
}

void	process_token_plus(t_frag_stack *frag_stack, int rule_index)
{
	if (is_empty_stack_frag(frag_stack))
	{
		fprintf(stderr, "Error: Stack is empty when processing plus token\n");
		exit(EXIT_FAILURE);
	}
	t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
	t_nfa_state *new_accept = init_nfa_state(-1, false, rule_index);
	t_nfa_transition *loop = malloc(sizeof(t_nfa_transition));
	if (!loop)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	loop->to = fragment->start;
	loop->symbol = '\0';
	add_transition(fragment->accept, loop);
	t_nfa_transition *exit_transition = malloc(sizeof(t_nfa_transition));
	if (!exit_transition)
	{
		perror("Failed to allocate memory for NFA transition");
		exit(EXIT_FAILURE);
	}
	exit_transition->to = new_accept;
	exit_transition->symbol = '\0';
	add_transition(fragment->accept, exit_transition);
	t_nfa_fragment *plus_fragment = init_nfa_fragment(fragment->start, new_accept);
	push_stack_frag(frag_stack, plus_fragment);
	free(fragment);
	fragment = NULL;
}