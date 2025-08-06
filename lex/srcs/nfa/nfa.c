#include "../../includes/lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_nfa_state_id = 0;

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

static t_nfa_fragment	*process_rule(t_token *current_token, int rule_index)
{
	t_frag_stack	*frag_stack = stack_create();
	while (current_token)
	{
		t_nfa_fragment *fragment = NULL;

		// printf("Processing token: %s\n", current_token->value);
		if (current_token->type == TOKEN_ESCAPE)
			fragment = process_token_escape(current_token, rule_index);
		else if (current_token->type == TOKEN_CHAR)
			fragment = process_token_char(current_token, rule_index);
		else if (current_token->type == TOKEN_KLEENE_STAR)
			process_token_kleene_star(frag_stack, rule_index);
		else if (current_token->type == TOKEN_PLUS)
			process_token_plus(frag_stack, rule_index);
		else if (current_token->type == TOKEN_CONCAT)
			process_token_concat(frag_stack);
		else if (current_token->type == TOKEN_ALTERNATION)
			process_token_alternation(frag_stack, rule_index);
		else if (current_token->type == TOKEN_OPTIONAL)
			process_token_optional(frag_stack, rule_index);
		else if (current_token->type == TOKEN_CLASS)
			fragment = process_token_class(current_token, rule_index);
		else if (current_token->type == TOKEN_QUANTIFIER)
			process_token_quantifier(frag_stack, current_token, rule_index);
		else if (current_token->type == TOKEN_QUOTE)
			fragment = process_token_quote(current_token, rule_index);
		if (fragment)
		{
			push_stack_frag(frag_stack, fragment);
			// printf("Pushed fragment to stack, current stack size: %zu\n", frag_stack->top);
		}
		current_token = current_token->next;
	}
	// printf("Popping final fragment from stack\n");
	t_nfa_fragment	*final_fragment = pop_stack_frag(frag_stack);
	// printf("Final fragment start state ID: %d\n", final_fragment->start->id);
	final_fragment->accept->is_accept = true;
	free_frag_stack(frag_stack);
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
	t_nfa_state *super_start = create_new_state();
	// printf("rule_count: %zu\n", rule_count);
	for (size_t i = 0; i < rule_count; i++)
	{
		// printf("Processing rule %zu\n", i);
		t_token *current_token = lex->rpn_list[i];
		rule_frags[i] = process_rule(current_token, i);
		rule_frags[i]->accept->rule_index = i;
	}
	add_epsilon_transitions(super_start, rule_frags, rule_count);
	lex->super_start = super_start;
	for (size_t i = 0; i < rule_count; i++)
	{
		free(rule_frags[i]);
	}
	free(rule_frags);
}

void	build_nfa(t_lex *lex)
{
	init_token_list(&lex->token_list, lex->rules_list.count);
    init_token_list(&lex->rpn_list, lex->rules_list.count);
    tokenize_patterns(lex);
    add_concat_tokens(lex);
    rpn(lex);
    build_nfa_from_rpn(lex);
	// run_test_suites(lex);
}
