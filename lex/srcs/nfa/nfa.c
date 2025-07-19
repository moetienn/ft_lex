#include "../../includes/lex.h"

static void	validate_rpn_list(t_lex *lex)
{
	if (!lex->rpn_list)
	{
		fprintf(stderr, "Error: RPN list is not initialized\n");
		exit(EXIT_FAILURE);
	}
}

static t_nfa_fragment	**initialize_rule_fragments(size_t rule_count)
{
	t_nfa_fragment	**rule_frags = malloc(sizeof(t_nfa_fragment*) * rule_count);
	if (!rule_frags)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	return (rule_frags);
}

static t_nfa_fragment	*process_rule(t_token *current_token)
{
	t_frag_stack	*frag_stack = stack_create();

	while (current_token)
	{
		if (current_token->type == TOKEN_CHAR)
		{
			t_nfa_fragment	*fragment = process_token_char(current_token);
			push_stack_frag(frag_stack, fragment);
		}
		else if (current_token->type == TOKEN_PLUS)
			process_token_plus(frag_stack);
		else if (current_token->type == TOKEN_CONCAT)
			process_token_concat(frag_stack);
		current_token = current_token->next;
	}
	t_nfa_fragment	*final_fragment = pop_stack_frag(frag_stack);
	final_fragment->accept->is_accept = true;
	return (final_fragment);
}

static void	add_epsilon_transitions(t_nfa_state *super_start, t_nfa_fragment **rule_frags, size_t rule_count)
{
	for (size_t i = 0; i < rule_count; ++i)
	{
		t_nfa_transition	*epsilon = malloc(sizeof(t_nfa_transition));
		if (!epsilon)
		{
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}
		epsilon->to = rule_frags[i]->start;
		epsilon->symbol = '\0';
		add_transition(super_start, epsilon);
	}
}

void	build_nfa_from_rpn(t_lex *lex)
{
	validate_rpn_list(lex);
	size_t rule_count = lex->rules_list.count;
	t_nfa_fragment **rule_frags = initialize_rule_fragments(rule_count);
	for (size_t i = 0; i < rule_count; i++)
	{
		t_token *current_token = lex->rpn_list[i];
		t_nfa_fragment *final_fragment = process_rule(current_token);
		final_fragment->accept->rule_index = i;
		rule_frags[i] = final_fragment;
	}
	t_nfa_state *super_start = create_new_state();
	add_epsilon_transitions(super_start, rule_frags, rule_count);
	lex->nfa_frag = malloc(sizeof(t_nfa_fragment));
	if (!lex->nfa_frag)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	lex->nfa_frag->start = super_start;
	lex->nfa_frag->accept = NULL;
	lex->all_rules_frags = rule_frags;
}

void	build_nfa(t_lex *lex)
{
	init_token_list(&lex->token_list, lex->rules_list.count);
	init_token_list(&lex->rpn_list, lex->rules_list.count);
	tokenize_patterns(lex);
	add_concat_tokens(lex);
	rpn(lex);
	init_nfa(lex);
	build_nfa_from_rpn(lex);
	run_test_suites(lex);
}