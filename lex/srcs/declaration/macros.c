#include "../../includes/lex.h"

char	*extract_macro_name(const char *section, size_t *i)
{
	size_t	start = *i;
	char	*name = NULL;

	while (section[*i] && section[*i] != ' ' && section[*i] != '[')
	{
		(*i)++;
	}
	size_t len = *i - start;
	name = malloc(len + 1);
	if (!name)
	{
		perror("Memory allocation failed");
		return NULL;
	}
	strncpy(name, &section[start], len);
	name[len] = '\0';
	return name;
}

char	*extract_macro_value(const char *section, size_t *i)
{
	size_t	start = *i;
	char	*value = NULL;

	while (section[*i] && section[*i] != '\n')
	{
		(*i)++;
	}
	size_t len = *i - start;
	value = malloc(len + 1);
	if (!value)
	{
		perror("Memory allocation failed");
		return NULL;
	}
	strncpy(value, &section[start], len);
	value[len] = '\0';
	return (value);
}

void	collect_macros(t_lex *lex, size_t *i)
{
	char	*name = NULL;
	char	*value = NULL;

	while (lex->parser.first_section[*i])
	{
		if (lex->parser.first_section[*i] != ' ' && lex->parser.first_section[*i] != '\t' && lex->parser.first_section[*i] != '\n' && lex->parser.first_section[*i] != '[')
			name = extract_macro_name(lex->parser.first_section, i);
		if (lex->parser.first_section[*i] == '[')
			value = extract_macro_value(lex->parser.first_section, i);
		if (name != NULL && value != NULL)
		{
			add_macro(&lex->macros_list, name, value);
			free(name);
			free(value);
			name = NULL;
			value = NULL;
		}
		(*i)++;
	}
	if (name != NULL)
		free(name);
	if (value != NULL)
		free(value);
}
