#include "../includes/lex.h"
#include "../includes/declaration/first_section.h"

void	init_lex(t_lex *lex)
{
	if (!lex)
		return;
	lex->declaration_code = NULL;
	lex->macros_list.list = NULL;
	lex->macros_list.count = 0;
	lex->macros_list.capacity = 0;
	lex->user_code = NULL;
	init_macros_list(&lex->macros_list);
	init_rules_list(&lex->rules_list);
}

void    ft_lex(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening file");
		return;
	}
	t_parser	parser;
	t_lex		lex;

	parser.file = file;
	parser.filename = strdup(filename);
	parse_file(&parser);
	init_lex(&lex);
	lex.parser = parser;
	collect_first_section(&parser, &lex);
	collect_rules(&lex);
	lex.user_code = strdup(parser.third_section);
	free_parser(&parser);
	build_nfa(&lex);
	if (lex.super_start)
		from_nfa_to_dfa(&lex);
	else
		fprintf(stderr, "Error: NFA not built correctly.\n");
	free_nfa_state_iterative(lex.super_start);
	free_lex(&lex);
}
