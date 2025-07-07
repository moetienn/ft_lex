#include "../../includes/parser.h"

int	init_parser(t_parser *parser)
{
	parser->delimiter_count = 0;
	parser->current_section = SEC_DECLARATIONS;
	parser->buffer = NULL;
	parser->buffer = malloc(1);
	parser->buffer[0] = '\0';
	parser->line = NULL;
	parser->len = 0;
	if (!parser->buffer)
	{
		fprintf(stderr, "Error: Memory allocation failed.\n");
		fclose(parser->file);
		return (1);
	}
	parser->buffer[0] = '\0'; // Initialize buffer to empty string
	printf("Parser initialized\n");
	return (0);
}