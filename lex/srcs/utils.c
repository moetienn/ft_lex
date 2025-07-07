#include "../includes/parser.h"

void    free_parser(t_parser *parser)
{
	if (!parser)
		return;
	if (parser->buffer)
		free(parser->buffer);
	if (parser->line)
		free(parser->line);
	if (parser->filename)
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
