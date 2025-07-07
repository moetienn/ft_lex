#include "../../includes/parser.h"

void	trim_whitespace(char *line, size_t len)
{
	while (len > 0 && (line[len - 1] == ' ' || line[len - 1] == '\t' || line[len - 1] == '\n' || line[len - 1] == '\r'))
		line[--len] = '\0';
	while (line[0] == ' ' || line[0] == '\t')
		memmove(line, line + 1, len--);
}

void append_to_buffer(t_parser *parser, const char *line)
{
    size_t needed = strlen(parser->buffer) + strlen(line) + 2;
    parser->buffer = realloc(parser->buffer, needed);
    strcat(parser->buffer, line);
    strcat(parser->buffer, "\n");
}

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
