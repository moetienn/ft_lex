#include "../includes/lex.h"
#include "../includes/first_section/first_section.h"

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
	lex.macros_list.list = NULL;
	lex.macros_list.count = 0;
	lex.macros_list.capacity = 0;

	// collect declarations
	collect_first_section(&parser, &lex);
	// collect rules
	// collect_second_section(&parser);
	// collect user code
	// collect_third_section(&parser);

	free_parser(&parser);
	free_lex(&lex);
	// end of processing
}