#include "../../includes/lex.h"

t_frag_stack *stack_create()
{
	t_frag_stack *stack = malloc(sizeof(t_frag_stack));
	if (!stack)
	{
		perror("Failed to allocate memory for fragment stack");
		exit(EXIT_FAILURE);
	}
	stack->fragments = NULL;
	stack->top = 0;
	stack->capacity = 0;
	return stack;
}

t_nfa_fragment *pop_stack_frag(t_frag_stack *stack)
{
	if (stack->top == 0)
	{
		fprintf(stderr, "Error: Attempt to pop from an empty stack\n");
		exit(EXIT_FAILURE);
	}
	return stack->fragments[--stack->top];
}

void    push_stack_frag(t_frag_stack *stack, t_nfa_fragment *fragment)
{
	if (stack->top >= stack->capacity)
	{
		size_t new_capacity = stack->capacity == 0 ? 1 : stack->capacity * 2;
		t_nfa_fragment **new_fragments = realloc(stack->fragments, new_capacity * sizeof(t_nfa_fragment *));
		if (!new_fragments)
		{
			perror("Failed to reallocate memory for fragment stack");
			exit(EXIT_FAILURE);
		}
		stack->fragments = new_fragments;
		stack->capacity = new_capacity;
	}
	stack->fragments[stack->top++] = fragment;
}

void	process_token_plus(t_frag_stack *frag_stack)
{
    t_nfa_fragment *fragment = pop_stack_frag(frag_stack);
    t_nfa_state *new_accept = init_nfa_state(-1, false);

    t_nfa_transition *loop = malloc(sizeof(t_nfa_transition));
    if (!loop)
    {
        perror("Failed to allocate memory for NFA transition");
        exit(EXIT_FAILURE);
    }
    loop->to = fragment->start;
    loop->symbol = '\0';
    add_transition(fragment->accept, loop);
    t_nfa_transition *exit = malloc(sizeof(t_nfa_transition));
    exit->to = new_accept;
    exit->symbol = '\0';
    add_transition(fragment->accept, exit);
    t_nfa_fragment *plus_fragment = init_nfa_fragment(fragment->start, new_accept);
    push_stack_frag(frag_stack, plus_fragment);
}

void	build_nfa_from_rpn(t_lex *lex)
{
	init_nfa(lex);
	if (!lex->rpn_list)
	{
		fprintf(stderr, "Error: RPN list is not initialized\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		t_token *current_token = lex->rpn_list[i];
		t_frag_stack *frag_stack = stack_create();

		while (current_token)
		{
			if (current_token->type == TOKEN_CHAR)
			{
				t_nfa_fragment *fragment = process_token_char(current_token);
				push_stack_frag(frag_stack, fragment);
				// printf("[main] Pushed fragment: start=%p, accept=%p\n", fragment->start, fragment->accept);
			}
			else if (current_token->type == TOKEN_PLUS)
			{
				process_token_plus(frag_stack);
			}
			else if (current_token->type == TOKEN_CONCAT)
			{
			    t_nfa_fragment *right = pop_stack_frag(frag_stack);
			    t_nfa_fragment *left = pop_stack_frag(frag_stack);
			    t_nfa_transition *epsilon = malloc(sizeof(t_nfa_transition));
			    epsilon->to = right->start;
			    epsilon->symbol = '\0';
			    add_transition(left->accept, epsilon);
			    left->accept->is_accept = false;
			    t_nfa_fragment *concat_fragment = init_nfa_fragment(left->start, right->accept);
			    push_stack_frag(frag_stack, concat_fragment);
			}
			current_token = current_token->next;
		}
		t_nfa_fragment *final_fragment = pop_stack_frag(frag_stack);
		// printf("[main] Poped fragment: start=%p, accept=%p\n", final_fragment->start, final_fragment->accept);
		final_fragment->accept->is_accept = true;
		lex->nfa_frag = final_fragment;
	}
}

void	build_nfa(t_lex *lex)
{
	init_token_list(&lex->token_list, lex->rules_list.count);
	init_token_list(&lex->rpn_list, lex->rules_list.count);
	tokenize_patterns(lex);
	add_concat_tokens(lex);
	rpn(lex);
	build_nfa_from_rpn(lex);
	printf("Test 'a': %s\n", test_nfa(lex->nfa_frag->start, "a") ? "ACCEPTED" : "REJECTED");
	printf("Test 'aa': %s\n", test_nfa(lex->nfa_frag->start, "aa") ? "ACCEPTED" : "REJECTED");
	printf("Test 'aaa': %s\n", test_nfa(lex->nfa_frag->start, "aaa") ? "ACCEPTED" : "REJECTED");
	printf("Test 'aaaa': %s\n", test_nfa(lex->nfa_frag->start, "aaaa") ? "ACCEPTED" : "REJECTED");
	printf("Test 'ab': %s\n", test_nfa(lex->nfa_frag->start, "ab") ? "ACCEPTED" : "REJECTED");
	printf("Test 'aab': %s\n", test_nfa(lex->nfa_frag->start, "aab") ? "ACCEPTED" : "REJECTED");
	printf("Test 'aaab': %s\n", test_nfa(lex->nfa_frag->start, "aaab") ? "ACCEPTED" : "REJECTED");
	printf("Test 'b': %s\n", test_nfa(lex->nfa_frag->start, "b") ? "ACCEPTED" : "REJECTED");
	printf("Test 'abb': %s\n", test_nfa(lex->nfa_frag->start, "abb") ? "ACCEPTED" : "REJECTED");
	printf("Test 'abc': %s\n", test_nfa(lex->nfa_frag->start, "abc") ? "ACCEPTED" : "REJECTED");
}