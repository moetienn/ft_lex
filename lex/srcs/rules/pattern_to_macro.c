#include "../../includes/lex.h"

char    *replace_tmp_with_macro(const char *tmp, t_macros_list *macros_list)
{
	for (size_t i = 0; i < macros_list->count; i++)
	{
		// printf("tmp macro: %s\n", tmp);
		// printf("macros_list->list[i].name: %s\n", macros_list->list[i].name);
		if (strcmp(macros_list->list[i].name, tmp) == 0)
		{
			char *macro_value = strdup(macros_list->list[i].value);
			if (!macro_value)
			{
				perror("Memory allocation failed");
				return NULL;
			}
			return (macro_value);
		}
	}
	return (strdup(tmp));
}

char	*append_macro(const char *str, size_t *i, char *macro, size_t start)
{
	while (str[*i] && !is_whitespace(str[*i]))
		(*i)++;
	size_t rest_len = *i - start;
	macro = realloc(macro, strlen(macro) + rest_len + 1);
	if (!macro)
	{
		perror("Memory allocation failed");
		return NULL;
	}
	strncat(macro, &str[start], rest_len);
	return macro;
}

int	get_macro_line(t_lex *lex, const char *macro_name)
{
	bool found = true;

	for (size_t i = 0; lex->parser.error.file_lines[i] != NULL; i++)
	{
		if (strstr(lex->parser.error.file_lines[i], macro_name) != NULL)
		{
			size_t j = 0;
			while (lex->parser.error.file_lines[i][j])
			{
				// printf("Checking character: %c\n", lex->parser.error.file_lines[i][j]);
				if (lex->parser.error.file_lines[i][j] != macro_name[j])
				{
					if (is_whitespace(lex->parser.error.file_lines[i][j]) && macro_name[j] == '\0')
					{
						// printf("Whitespace found at index %zu, macro name: %s\n", j, macro_name);
						return i + 1; // Return line number (1-based index)
					}
					// printf("char difference at index %zu: '%d' vs '%d'\n", j, lex->parser.error.file_lines[i][j], macro_name[j]);
					// printf("Macro %s not found in line %zu\n", macro_name, i + 1);
					found = false;
					break;
				}
				j++;
			}
			if (found == false)
				continue;
			return i + 1; // Return line number (1-based index)
		}
	}
	return 0; // Return 0 if not found
}

char	*pattern_to_macro(const char *str, size_t *i, t_macros_list *macros_list, t_lex *lex)
{
	char    *tmp = NULL;
	char    *macro = NULL;
	size_t  start = *i + 1;

	while (str[*i] && str[*i] != '}')
		(*i)++;
	size_t len = *i - start;
	tmp = malloc(len + 1);
	if (!tmp)
	{
		perror("Memory allocation failed");
		return NULL;
	}
	strncpy(tmp, &str[start], len);
	tmp[len] = '\0';
	macro = replace_tmp_with_macro(tmp, macros_list);
	if (strlen(macro) == 0)
	{
		printf("Error: Empty macro definition for '%s'\n", tmp);
		lex->parser.error_line = get_macro_line(lex, tmp);
		fprintf(stderr, "%s:%d: incomplete name definition\n", lex->parser.filename, lex->parser.error_line);
	}
	if (str[*i] == '}' && !is_whitespace(str[*i + 1]) && str[*i + 1] != '\0' && strlen(macro) > 0)
	{
		start = *i + 1;
		macro = append_macro(str, i, macro, start);
		if (!macro)
			return (free(tmp), NULL);
	}
	return (free(tmp), macro);
}