#include "../../includes/lex.h"

// CLEAN THIS FUNCTION. TOO BIG, TOO LONG

char	*extract_pattern(const char *str, size_t *i)
{
	char	*pattern = NULL;
	size_t	start = *i;
	bool	opened = false;

	if (str[*i] == '(' || str[*i] == '[')
		opened = true;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		opened = true;
		(*i)++;
	}
	while (str[*i] && (!is_whitespace(str[*i]) || opened == true))
	{
		if (str[*i] == ')' || str[*i] == ']')
			opened = false;
		if ((str[*i] == '\'' || str[*i] == '\"') && opened == true)
		{
			opened = false;
			(*i)++;
			if (is_whitespace(str[*i]))
				break;
		}
		if ((str[*i] == '\'' || str[*i] == '\"') && opened == false)
		{
			opened = true;
			(*i)++;
		}
		(*i)++;
	}
	size_t len = *i - start;
	pattern = malloc(len + 1);
	if (!pattern)
	{
		perror("Memory allocation failed");
		return NULL;
	}
	strncpy(pattern, &str[start], len);
	pattern[len] = '\0';
	return (pattern);
}
