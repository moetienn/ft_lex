#include "../../includes/lex.h"

void	class_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	printf("token class\n");
	size_t start = *j;
	while (lex->rules_list.list[i].pattern[*j] != ']' && lex->rules_list.list[i].pattern[*j] != '\0')
		(*j)++;
	if (lex->rules_list.list[i].pattern[*j] == ']')
	{
		current->value = malloc(*j - start + 2); // Allocate memory for the token value
		if (!current->value)
		{
			perror("Memory allocation failed for class token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start + 1);
		current->value[*j - start + 1] = '\0';
	}
}

void	plus_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	size_t start = *j;
	while (lex->rules_list.list[i].pattern[*j] == '+' && lex->rules_list.list[i].pattern[*j] != '\0')
		(*j)++;
	if (*j > start)
	{
		current->value = realloc(current->value, *j - start + 1);
		if (!current->value)
		{
			perror("Memory allocation failed for plus token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start);
		current->value[*j - start] = '\0';
	}
	else
	{
		current->value = strdup("+");
		if (!current->value)
		{
			perror("Memory allocation failed for plus token value");
			exit(EXIT_FAILURE);
		}
	}
}

void	quote_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	size_t start = *j;
	*j += 1;
	while (lex->rules_list.list[i].pattern[*j] != '"' && lex->rules_list.list[i].pattern[*j] != '\0')
		(*j)++;
	if (lex->rules_list.list[i].pattern[*j] == '"')
	{
		current->value = malloc(*j - start + 2);
		if (!current->value)
		{
			perror("Memory allocation failed for quote token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start + 1);
		current->value[*j - start + 1] = '\0';
	}

}

void	alternation_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	size_t start = *j;
	while (lex->rules_list.list[i].pattern[*j] == '|' && lex->rules_list.list[i].pattern[*j] != '\0')
		(*j)++;
	if (*j > start)
	{
		current->value = realloc(current->value, *j - start + 1);
		if (!current->value)
		{
			perror("Memory allocation failed for alternation token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start);
		current->value[*j - start] = '\0';
	}
	else
	{
		current->value = strdup("|");
		if (!current->value)
		{
			perror("Memory allocation failed for alternation token value");
			exit(EXIT_FAILURE);
		}
	}
	(*j)--;
}
