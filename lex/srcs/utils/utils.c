#include "../../includes/lex.h"
#include "../../includes/parser.h"

bool    is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

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

void    free_lex(t_lex *lex)
{
    if (!lex)
        return;
    if (lex->declaration_code)
        free(lex->declaration_code);
    free_macros_list(&lex->macros_list);
}
