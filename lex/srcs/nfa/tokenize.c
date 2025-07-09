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

void	tokenize_patterns(t_lex *lex)
{
	printf("Tokenizing patterns...\n");
	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		t_token *current = NULL;
		for (size_t j = 0; lex->rules_list.list[i].pattern[j] != '\0'; j++)
		{
			char c = lex->rules_list.list[i].pattern[j];
			t_token_type type = get_token_type(c);

			if (!current)
				initialize_first_token(&lex->token_list[i], &current, type);
			else
				add_next_token(&current, type, NULL);
			if (type == TOKEN_CLASS)
				class_token(current, lex, i, &j);
			else if (type == TOKEN_PLUS)
				plus_token(current, lex, i, &j);
		}
	}
	printf("Tokenization complete.\n");
	// print tokens for debugging
	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		printf("Tokens for Rule %zu:\n", i);
		t_token *current = lex->token_list[i];
		while (current)
		{
			printf("  Type: %d, Value: %s\n", current->type, current->value ? current->value : "NULL");
			current = current->next;
		}
	}
}