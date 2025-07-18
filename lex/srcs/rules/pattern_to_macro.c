#include "../../includes/lex.h"

char    *replace_tmp_with_macro(const char *tmp, t_macros_list *macros_list)
{
	for (size_t i = 0; i < macros_list->count; i++)
	{
		if (strcmp(macros_list->list[i].name, tmp) == 0)
			return (strdup(macros_list->list[i].value));
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

char	*pattern_to_macro(const char *str, size_t *i, t_macros_list *macros_list)
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
	if (str[*i] == '}' && !is_whitespace(str[*i + 1]) && str[*i + 1] != '\0')
	{
		start = *i + 1;
		macro = append_macro(str, i, macro, start);
		if (!macro)
			return (free(tmp), NULL);
	}
	return (free(tmp), macro);
}