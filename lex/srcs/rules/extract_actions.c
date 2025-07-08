#include "../../includes/lex.h"

char    *extract_action(const char *str, size_t *i)
{
	char    *action = NULL;
	size_t  start = *i;

	while (str[*i] && str[*i] != '}')
		(*i)++;
	size_t len = *i + 1 - start;
	action = malloc(len + 1);
	if (!action)
	{
		perror("Memory allocation failed");
		return NULL;
	}
	strncpy(action, &str[start], len);
	action[len] = '\0';
	return (action);
}
