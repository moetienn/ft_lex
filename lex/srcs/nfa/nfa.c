#include "../../includes/lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	// printf("[DEBUG] Created fragment stack %p\n", (void *)frag_stack);

	while (current_token)
	{
		t_nfa_fragment *fragment = NULL;

		if (current_token->type == TOKEN_CHAR)
		{
			fragment = process_token_char(current_token);
		}
		else if (current_token->type == TOKEN_KLEENE_STAR)
		{
			process_token_kleene_star(frag_stack);
		}
		else if (current_token->type == TOKEN_PLUS)
		{
			process_token_plus(frag_stack);
		}
		else if (current_token->type == TOKEN_CONCAT)
		{
			process_token_concat(frag_stack);
		}

		if (fragment)
		{
			push_stack_frag(frag_stack, fragment);
		}
		current_token = current_token->next;
	}
	t_nfa_fragment	*final_fragment = pop_stack_frag(frag_stack);
	final_fragment->accept->is_accept = true;
	free_frag_stack(frag_stack);
	return (final_fragment);
}

t_nfa_state	*deep_copy_nfa_state(t_nfa_state *state)
{
	t_nfa_state *new_state = malloc(sizeof(t_nfa_state));
	if (!new_state)
	{
		perror("Failed to allocate memory for NFA state");
		exit(EXIT_FAILURE);
	}
	new_state->id = state->id;
	new_state->rule_index = state->rule_index;
	new_state->is_accept = state->is_accept;
	new_state->transitions = malloc(sizeof(t_nfa_transition*) * state->transition_capacity);
	if (!new_state->transitions)
	{
		perror("Failed to allocate memory for NFA transitions");
		free(new_state);
		exit(EXIT_FAILURE);
	}
	memcpy(new_state->transitions, state->transitions, sizeof(t_nfa_transition*) * state->transition_count);
	new_state->transition_count = state->transition_count;
	new_state->transition_capacity = state->transition_capacity;
	new_state->visited = state->visited;
	return new_state;
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
		printf("[add_epsilon_transitions] Adding epsilon transition from super_start %p to rule_frags[%zu]->start %p\n", (void *)super_start, i, (void *)rule_frags[i]->start);
		add_transition(super_start, epsilon);
	}
}

void	build_nfa_from_rpn(t_lex *lex)
{
	validate_rpn_list(lex);
	size_t rule_count = lex->rules_list.count;
	t_nfa_fragment **rule_frags = initialize_rule_fragments(rule_count);
	// printf("[**RULE FRAGS] init **rule_frags %p\n", (void *)rule_frags);
	for (size_t i = 0; i < rule_count; i++)
	{
		t_token *current_token = lex->rpn_list[i];
		rule_frags[i] = process_rule(current_token);
		rule_frags[i]->accept->rule_index = i;
	}
	t_nfa_state *super_start = create_new_state();
	add_epsilon_transitions(super_start, rule_frags, rule_count);
	lex->super_start = super_start;
	lex->all_rules_frags = rule_frags;
}

void	build_nfa(t_lex *lex)
{
	init_token_list(&lex->token_list, lex->rules_list.count);
    init_token_list(&lex->rpn_list, lex->rules_list.count);
    tokenize_patterns(lex);
    add_concat_tokens(lex);
    rpn(lex);
	// from here leaks occur
    build_nfa_from_rpn(lex);
    run_test_suites(lex);
	free_all_rules_frags(lex->all_rules_frags, lex->rules_list.count);
	free_nfa_state_recursive(lex->super_start);
}
