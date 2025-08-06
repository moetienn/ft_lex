#include "../../includes/lex.h"
#include "../../includes/nfa/nfa.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	const char *input;
	bool expected;
} t_nfa_test;

typedef struct {
	t_nfa_state **states;
	size_t count;
	size_t capacity;
} state_set;

void	add_state(state_set *set, t_nfa_state *state)
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

void    free_state_set(state_set *set)
{
	free(set->states);
	set->states = NULL;
	set->count = set->capacity = 0;
}

void	test_epsilon_closure(state_set *set, t_nfa_state *state)
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
				test_epsilon_closure(set, t->to);
		}
	}
}

int test_nfa_rule_index(t_nfa_state *start_state, const char *input)
{
	state_set current = {0}, next = {0};
	test_epsilon_closure(&current, start_state);

	const char *ptr = input;

	while (*ptr)
	{
		next.count = 0;

		for (size_t i = 0; i < current.count; i++)
		{
			t_nfa_state *state = current.states[i];
			for (size_t t = 0; t < state->transition_count; t++)
			{
				t_nfa_transition *trans = state->transitions[t];
				if (trans->symbol == *ptr)
				{
					test_epsilon_closure(&next, trans->to);
				}
			}
		}
		if (next.count == 0)
		{
			free_state_set(&current);
			free_state_set(&next);
			return -1;
		}
		state_set tmp = current;
		current = next;
		next = tmp;
		ptr++;
	}
	next.count = 0;
	for (size_t i = 0; i < current.count; ++i)
		test_epsilon_closure(&next, current.states[i]);

	int found_rule = -1;
	for (size_t i = 0; i < next.count; i++)
	{
		if (next.states[i]->is_accept)
		{
			int rule = next.states[i]->rule_index;
			if (found_rule == -1 || rule < found_rule)
				found_rule = rule;
		}
	}
	free_state_set(&current);
	free_state_set(&next);
	return found_rule;
}

bool    test_nfa(t_nfa_state *start_state, const char *input)
{
	return test_nfa_rule_index(start_state, input) != -1;
}

void    run_nfa_test_suite(
	t_nfa_state *start_state,
	const t_nfa_test *tests,
	size_t n_tests,
	const char *test_suite_name
) {
	const char *CHECK = "✅";
	const char *CROSS = "❌";
	const char *GREEN = "\033[32m";
	const char *RED = "\033[31m";
	const char *RESET = "\033[0m";

	printf("\n=== Test suite: %s ===\n", test_suite_name);
	for (size_t i = 0; i < n_tests; ++i) {
		bool result = test_nfa(start_state, tests[i].input);
		bool ok = (result == tests[i].expected);
		printf("Test '%s': %s %s%s%s\n",
			tests[i].input,
			result ? "ACCEPTED" : "REJECTED",
			ok ? GREEN : RED,
			ok ? CHECK : CROSS,
			RESET
		);
	}
}

void    run_test_suites(t_lex *lex)
{
	t_nfa_test digit_tests[] = {
		{"1", true}, {"9", true}, {"1234567890", true},
		{"0123", true}, {"", false}, {"a", false}, {"1a", false}, {"a1", false},
		{"123abc", false}, {"abc123", false}, {"1 2 3", false},
	};

	// Tests pour {TEST}+ : n'accepte que des suites de "abc" collées
	t_nfa_test test_tests[] = {
		{"abc", true}, {"abcabc", true}, {"abcabcabc", true},
		{"", false}, {"ab", false}, {"a", false}, {"bc", false},
		{"abca", false}, {"abcabcab", false}, {"abcabcabca", false},
	};

	// Tests croisés : 
	t_nfa_test mixed_tests[] = {
		{"123abc", false}, {"abc123", false}, {"abc1", false}, {"1abc", false},
		{"abcabc123", false}, {"123abcabc", false}, {"abcabcabcabc", true},
		{"123", true}, {"abc", true}, {"", false},
	};


	run_nfa_test_suite(lex->super_start, digit_tests, sizeof(digit_tests)/sizeof(*digit_tests), "DIGIT+");
	run_nfa_test_suite(lex->super_start, test_tests, sizeof(test_tests)/sizeof(*test_tests), "TEST+");
	run_nfa_test_suite(lex->super_start, mixed_tests, sizeof(mixed_tests)/sizeof(*mixed_tests), "Mixed inputs (DIGIT+)");
	run_nfa_test_suite(lex->super_start, mixed_tests, sizeof(mixed_tests)/sizeof(*mixed_tests), "Mixed inputs (TEST+)");

	// t_nfa_test tests_a_plus_b[] = {
	// 	{"ab", true}, {"aab", true}, {"aaab", true}, {"a", false}, {"b", false}, {"aa", false},
	// };
	// size_t n_tests_a_plus_b = sizeof(tests_a_plus_b) / sizeof(*tests_a_plus_b);
	// run_nfa_test_suite(lex->super_start, tests_a_plus_b, n_tests_a_plus_b, "a+b");

	// t_nfa_test tests_a_star[] = {
	// 	{"", true}, {"a", true}, {"aa", true}, {"aaa", true}, {"b", false}, {"ab", false}, {"aab", false}, {"aaab", false}, {"bba", false},
	// };
	// size_t n_tests_a_star = sizeof(tests_a_star) / sizeof(*tests_a_star);
	// run_nfa_test_suite(lex->super_start, tests_a_star, n_tests_a_star, "a*");

	// t_nfa_test tests_a_star_b[] = {
	// 	{"", false}, {"a", false}, {"aa", false}, {"aaa", false}, {"b", true}, {"ab", true}, {"aab", true}, {"aaab", true}, {"bba", false},
	// };
	// size_t n_tests_a_star_b = sizeof(tests_a_star_b) / sizeof(*tests_a_star_b);
	// run_nfa_test_suite(lex->super_start, tests_a_star_b, n_tests_a_star_b, "a*b");

	// t_nfa_test tests_a_alternation_b[] = {
	// 	{"a", true}, {"b", true}, {"aa", false}, {"aaa", false}, {"ab", false}, {"aab", false}, {"aaab", false}, {"bba", false},
	// };
	// size_t n_tests_a_alternation_b = sizeof(tests_a_alternation_b) / sizeof(*tests_a_alternation_b);
	// run_nfa_test_suite(lex->super_start, tests_a_alternation_b, n_tests_a_alternation_b, "a|b");

	// t_nfa_test tests_a_alternation_b_alternation_c[] = {
	// 	{"a", true}, {"b", true}, {"c", true}, {"aa", false}, {"aaa", false}, {"ab", false}, {"aab", false}, {"aaab", false}, {"bba", false}, {"ac", false}, {"bc", false}, {"abc", false}, {"aab", false}, {"bba", false}, {"cc", false}, {"cba", false}, {"cab", false}, {"caa", false}, {"bbc", false}, {"cbb", false}, {"ccc", false}, {"aabcc", false}, {"bbaac", false}, {"ccab", false}, {"abcabc", false},
	// };
	// size_t n_tests_a_alternation_b_alternation_c = sizeof(tests_a_alternation_b_alternation_c) / sizeof(*tests_a_alternation_b_alternation_c);
	// run_nfa_test_suite(lex->super_start, tests_a_alternation_b_alternation_c, n_tests_a_alternation_b_alternation_c, "a|b|c");

	// t_nfa_test tests_a_star_b_alternation_c[] = {
	// 	{"", false}, {"a", false}, {"aa", false}, {"aaa", false}, {"b", true}, {"ab", true}, {"aab", true}, {"aaab", true}, {"bba", false},
	// 	{"c", true}, {"ac", false}, {"bc", false}, {"abc", false}, {"aabcc", false}, {"bbaac", false}, {"ccab", false},
	// };
	// size_t n_tests_a_star_b_alternation_c = sizeof(tests_a_star_b_alternation_c) / sizeof(*tests_a_star_b_alternation_c);
	// run_nfa_test_suite(lex->super_start, tests_a_star_b_alternation_c, n_tests_a_star_b_alternation_c, "a*b|c");

	// t_nfa_test tests_a_optional[] = {
	// 	{"", true}, {"a", true}, {"aa", false}, {"aaa", false}, {"b", false}, {"ab", false}, {"aab", false}, {"aaab", false}, {"bba", false},
	// };
	// size_t n_tests_a_optional = sizeof(tests_a_optional) / sizeof(*tests_a_optional);
	// run_nfa_test_suite(lex->super_start, tests_a_optional, n_tests_a_optional, "a?");

	// t_nfa_test tests_a_b_optional_c_optional[] = {
	// 	{"", false}, {"a", true}, {"b", false}, {"c", false}, {"ab", true}, {"ac", true}, {"abc", true}, {"bc", false},
	// 	{"aab", false}, {"aaac", false}, {"bbac", false}, {"aabbcc", false},
	// 	{"aabc", false}, {"abcc", false}, {"bca", false}, {"cba", false},
	// };
	// size_t n_tests_a_b_optional_c_optional = sizeof(tests_a_b_optional_c_optional) / sizeof(*tests_a_b_optional_c_optional);
	// run_nfa_test_suite(lex->super_start, tests_a_b_optional_c_optional, n_tests_a_b_optional_c_optional, "ab?c?");

	// t_nfa_test tests_a_escape_t_b[] = {
    // {"a\tb", true}, {"ab", false}, {"a b", false}, {"a\nb", false}, {"a\r\nb", false},
    // {"a\\b", false}, {"aa", false}, {"bb", false}, {"aaa", false}, {"abab", false},
    // {"aab", false}, {"a'b", false}, {"a\"b", false}, {"a\fb", false}, {"a\bb", false}, };

	// size_t n_tests_a_escape_t_b = sizeof(tests_a_escape_t_b) / sizeof(*tests_a_escape_t_b);
	// run_nfa_test_suite(lex->super_start, tests_a_escape_t_b, n_tests_a_escape_t_b, "a\tb");

	// t_nfa_test tests_digit_plus[] = {
    // {"", false}, {"1", true}, {"9", true}, {"123", true}, {"000", true},
    // {"9876543210", true}, {"a", false}, {"1a", false}, {"a1", false},
    // {"12b34", false}, {"1 2", false}, {" 123", false}, {"123 ", false},
    // {"12.3", false}, {"-123", false}
	// };
	// size_t n_tests_digit_plus = sizeof(tests_digit_plus) / sizeof(*tests_digit_plus);
	// run_nfa_test_suite(lex->super_start, tests_digit_plus, n_tests_digit_plus, "digit+");

	// t_nfa_test tests_letter_star[] = {
    // {"", true}, {"a", true}, {"z", true}, {"abc", true}, {"azbyc", true},
    // {"aaaaa", true}, {"A", false}, {"Abc", false}, {"abcABC", false},
    // {"1", false}, {"a1b2c3", false}, {" ", false}, {"a b", false},
    // {"-", false}, {"az-", false}
	// };
	// size_t n_tests_letter_star = sizeof(tests_letter_star) / sizeof(*tests_letter_star);
	// run_nfa_test_suite(lex->super_start, tests_letter_star, n_tests_letter_star, "letter*");

	// t_nfa_test tests_id[] = {
    // {"a", true}, {"A", true}, {"_", true}, {"abc", true}, {"ABC", true},
    // {"a123", true}, {"_var123", true}, {"Abc_def", true}, {"a1b2c3", true},
    // {"variable_", true}, {"variable123", true}, {"_123", true}, {"a_b_c", true},
    // {"", false}, {"1abc", false}, {"9", false}, {"1", false}, {"123var", false},
    // {"-abc", false}, {" abc", false}, {"var-name", false}, {"a.b", false},
    // {"a b", false}, {"var$", false}
	// };
	// size_t n_tests_id = sizeof(tests_id) / sizeof(*tests_id);
	// run_nfa_test_suite(lex->super_start, tests_id, n_tests_id, "ID");
}