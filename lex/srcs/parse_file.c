#include "../includes/parser.h"

void	get_section(t_parser *parser)
{
	if (parser->current_section == SEC_DECLARATIONS)
	{
		parser->first_section = strdup(parser->buffer);
		parser->current_section = SEC_RULES;
	}
	else if (parser->current_section == SEC_RULES)
	{
		parser->second_section = strdup(parser->buffer);
		parser->current_section = SEC_USER_CODE;
	}
	else if (parser->current_section == SEC_USER_CODE)
	{
		parser->third_section = strdup(parser->buffer);
	}
	free(parser->buffer);
	parser->buffer = malloc(1024); // Reset buffer for next section
	parser->buffer[0] = '\0'; // Initialize buffer to empty string
}

void	parse_file(t_parser *parser)
{
	printf("Parsing file...\n");

	if (!check_parser(parser))
	{
		fprintf(stderr, "Error: Invalid parser state.\n");
		return;
	}

	if (init_parser(parser) != 0)
	{
		fprintf(stderr, "Error: Failed to initialize parser.\n");
		return;
	}

	// Read the file and get the sections
	while (getline(&parser->line, &parser->len, parser->file) != -1)
	{
		// printf("Read line: %s", parser->line);
		if (strstr(parser->line, "%%") != NULL)
			get_section(parser);
		else
			strcat(parser->buffer, parser->line);	
	}
	if (parser->current_section == SEC_USER_CODE && parser->buffer[0] != '\0')
		get_section(parser);
	if (parser->buffer != NULL)
	{
		free(parser->buffer);
		parser->buffer = NULL;
	}
	printf("Parsing completed.\n");
	printf("First section: %s\n", parser->first_section);
	printf("Second section: %s\n", parser->second_section);
	printf("Third section: %s\n", parser->third_section);
}

