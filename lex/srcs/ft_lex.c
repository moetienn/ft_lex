#include "../includes/lex.h"
#include "../includes/declaration/first_section.h"

void	init_lex(t_lex *lex)
{
	if (!lex)
		return;
	// Initialize declaration_code to NULL
	lex->declaration_code = NULL;
	lex->macros_list.list = NULL;
	lex->macros_list.count = 0;
	lex->macros_list.capacity = 0;
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
	// Process the file...
	t_parser	parser;
	t_lex		lex;

	parser.file = file;
	parser.filename = strdup(filename);
	parse_file(&parser);

	// Initialize macros in t_lex
	init_lex(&lex);

	// collect declarations
	lex.parser = parser;
	collect_first_section(&parser, &lex);
	// collect rules
	collect_rules(&lex);
	// collect user code
	// collect_third_section(&parser);

	free_parser(&parser);
	free_lex(&lex);
	// end of processing
}
