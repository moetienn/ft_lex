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
#include "dfa/dfa.h"

// MACROS
#undef CHECK_ALLOC
#define CHECK_ALLOC(ptr, cleanup, msg, ret_val) \
	if (!(ptr)) { \
		fprintf(stderr, msg "\n"); \
		cleanup; \
		return ret_val; \
	}

typedef struct s_lex
{
	char			*declaration_code;
	t_macros_list	macros_list;
	t_parser		parser;
	t_rules_list	rules_list;
	int				rule_index;
	char			*user_code;
	t_token			**token_list;
	t_token			**rpn_list;
	t_nfa_state		*super_start;
	t_dfa			*dfa;
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
t_nfa_state		*init_nfa_state(int id, bool is_accept, int rule_index);
t_nfa_fragment	*init_nfa_fragment(t_nfa_state *start, t_nfa_state *accept);

// nfa utils

t_nfa_state	*create_new_state(void);
t_frag_stack	*stack_create(void);
t_nfa_fragment *pop_stack_frag(t_frag_stack *stack);
void    push_stack_frag(t_frag_stack *stack, t_nfa_fragment *fragment);

// DFA

void	generate_lexyyc(t_lex *lex, const char *alphabet, int alphabet_size);
void    from_nfa_to_dfa(t_lex *lex);
void    compute_closure_epsilon(t_lex *lex, t_memory_stack *mem_stack);

// rpn utils
t_token	*create_token_copy(t_token *operand);
void	append_to_rpn_list(t_token **rpn_list, t_token *operand);
void	rpn(t_lex *lex);
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