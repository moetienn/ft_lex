#include "../../includes/first_section/first_section.h"
#include "../../includes/lex.h"

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
				(*i) += 3; // Skip the closing %}
				return ;
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
	lex->parser = *parser; // Copy parser to lex
	size_t i = 0;
	get_declaration_code(parser, lex, &i);
	// here we need to collect macros
	init_macros_list(&lex->macros_list);
	collect_macros(lex, &i);

	// print macros list for debugging
	printf("macros_list count: %zu\n", lex->macros_list.count);
	for (size_t j = 0; j < lex->macros_list.count; j++)
	{
		printf("Macro %zu: %s = %s\n", j + 1, lex->macros_list.list[j].name, lex->macros_list.list[j].value);
	}
}
