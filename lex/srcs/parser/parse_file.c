#include "../../includes/parser.h"

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
	parser->buffer = malloc(1);
	parser->buffer[0] = '\0';
}

void	get_section_loop(t_parser *parser)
{
	while (getline(&parser->line, &parser->len, parser->file) != -1)
	{
		size_t len = strlen(parser->line);
		add_to_error(&parser->error, parser->line, len);
		trim_whitespace(parser->line, len, parser);
		len = strlen(parser->line);
		if (len == 0)
			continue;
		if (parser->line[0] == '%' && parser->line[1] == '%')
			get_section(parser);
		else
			append_to_buffer(parser, parser->line);
		parser->error_line++;
	}
	if (parser->current_section == SEC_USER_CODE && parser->buffer[0] != '\0')
		get_section(parser);
	if (parser->buffer != NULL)
	{
		free(parser->buffer);
		parser->buffer = NULL;
	}
}

void	parse_file(t_parser *parser)
{
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
	get_section_loop(parser);
}

