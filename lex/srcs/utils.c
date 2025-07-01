#include "../includes/parser.h"

int	check_parser(t_parser *parser)
{
	if (!parser)
		return (0);
	if (!parser->file)
	{
		fprintf(stderr, "Error: File '%s' is not opened.\n", parser->filename);
		return (0);
	}
	return (1);
}

void    free_parser(t_parser *parser)
{
	if (!parser)
		return;
	free(parser->buffer);
	free(parser->line);
	free(parser->filename);
    if (parser->first_section)
        free(parser->first_section);
    if (parser->second_section)
        free(parser->second_section);
    if (parser->third_section)
        free(parser->third_section);
    if (parser->file)
        fclose(parser->file);
}
