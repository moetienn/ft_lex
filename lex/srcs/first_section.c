#include "../includes/first_section/first_section.h"
#include "../includes/lex.h"

void	get_declaration_code(t_parser *parser, t_lex *lex, size_t *i)
{
	while(parser->first_section[*i])
	{
		if (parser->first_section[*i] == '%' && parser->first_section[*i + 1] == '{')
		{
			*i += 2;
			if (parser->first_section[*i] == '\n')
				(*i)++;
			size_t start = *i;
			while (parser->first_section[*i] && !(parser->first_section[*i] == '%' && parser->first_section[*i + 1] == '}'))
				(*i)++;
			if (parser->first_section[*i] == '%' && parser->first_section[*i + 1] == '}')
			{
				size_t len = (*i - 1) - start;
				lex->declaration_code = malloc(len + 1);
				if (!lex->declaration_code)
				{
					perror("Memory allocation failed");
					return;
				}
				strncpy(lex->declaration_code, &parser->first_section[start], len);
				lex->declaration_code[len] = '\0';
			}
		}
		(*i)++;
	}
}

void    collect_first_section(t_parser *parser, t_lex *lex)
{
	if (!parser || !lex)
	{
		fprintf(stderr, "Error: Invalid parser or lex structure.\n");
		return;
	}
	// Initialize lex declaration_code
	lex->declaration_code = NULL;
	size_t i = 0;
	get_declaration_code(parser, lex, &i);
}
