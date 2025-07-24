#ifndef LEX_H
# define LEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "parser.h"
#include "declaration/macros.h"
#include "rules/rules.h"
#include "nfa/nfa.h"

// ----------- TEST STRUCT -----------
typedef struct {
    const char *input;
    bool expected; // true = ACCEPTED, false = REJECTED
} t_nfa_test;

typedef struct s_lex
{
	char			*declaration_code;
	t_macros_list	macros_list;
	t_parser		parser;
	t_rules_list	rules_list;
	char			*user_code;
	// NFA
	// create a linked list that hold the list of t_token *nfa
	// the NFA is a linked list of tokens
	t_token			**token_list; // linked list of tokens
	// RPN
	t_token			**rpn_list; // linked list of tokens in RPN
	// NFA
	t_nfa_state		*super_start; // start state of the NFA
}   t_lex;

// utils
bool    is_whitespace(char c);
void    free_lex(t_lex *lex);
void    file_error(const char *filename);

// Macros
void    collect_macros(t_lex *lex, size_t *i);
// Rules
char    *extract_action(const char *str, size_t *i);

char	*extract_pattern(const char *str, size_t *i);
char    *replace_tmp_with_macro(const char *tmp, t_macros_list *macros_list);
char	*append_macro(const char *str, size_t *i, char *macro, size_t start);
char	*pattern_to_macro(const char *str, size_t *i, t_macros_list *macros_list);
void    collect_rules(t_lex *lex);

// TOKEN IDENTIFICATION

void	class_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	plus_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	quote_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	alternation_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	kleene_star_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	escape_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	char_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	optional_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	left_paren_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	right_paren_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void	quantifier_token(t_token *current, t_lex *lex, size_t i, size_t *j);
void    add_concat_tokens(t_lex *lex);

// -------- NFA --------

// init nfa
t_nfa_state		*init_nfa_state(int id, bool is_accept);
t_nfa_fragment	*init_nfa_fragment(t_nfa_state *start, t_nfa_state *accept);

// nfa utils
t_nfa_state	*create_new_state(void);
t_frag_stack	*stack_create(void);
t_nfa_fragment *pop_stack_frag(t_frag_stack *stack);
void    push_stack_frag(t_frag_stack *stack, t_nfa_fragment *fragment);

// TESTER DONT FORGET TO REMOVE
bool	test_nfa(t_nfa_state *start_state, const char *input);
void run_nfa_test_suite(
    t_nfa_state *start_state,
    const t_nfa_test *tests,
    size_t n_tests,
    const char *test_suite_name
);
void    run_test_suites(t_lex *lex);
// rpn
void	rpn(t_lex *lex);
// rpn utils
t_token	*create_token_copy(t_token *operand);
void	append_to_rpn_list(t_token **rpn_list, t_token *operand);
// stack
void    init_stack(t_stack *stack, size_t capacity);
void	push_stack(t_stack *stack, t_token *token);
t_token	*pop_stack(t_stack *stack);
// shunting yard utils
bool	priority_check(t_token *current, t_stack *operator_stack);
bool    is_operand(t_token_type type);
bool    is_operator(t_token_type type);

void    tokenize_patterns(t_lex *lex);
void    build_nfa(t_lex *lex);

void	ft_lex(const char *filename);

#endif