#include "../../../includes/lex.h"

void	class_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
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
	(*j)--;
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

void	kleene_star_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	size_t start = *j;
	while (lex->rules_list.list[i].pattern[*j] == '*' && lex->rules_list.list[i].pattern[*j] != '\0')
		(*j)++;
	if (*j > start)
	{
		current->value = realloc(current->value, *j - start + 1);
		if (!current->value)
		{
			perror("Memory allocation failed for kleene star token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start);
		current->value[*j - start] = '\0';
	}
	else
	{
		current->value = strdup("*");
		if (!current->value)
		{
			perror("Memory allocation failed for kleene star token value");
			exit(EXIT_FAILURE);
		}
	}
	(*j)--;
}

void	escape_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
    size_t start = *j;
    *j += 1;
    if (lex->rules_list.list[i].pattern[*j] == '\0')
    {
        fprintf(stderr, "Error: Escape sequence at end of pattern in rule %zu\n", i);
        exit(EXIT_FAILURE);
    }
    else
    {
        size_t escape_length = 2; // Escape sequence length (e.g., \n, \t)
        current->value = malloc(escape_length + 1); // Allocate memory for escape sequence + null terminator
        if (!current->value)
        {
            perror("Memory allocation failed for escape token value");
            exit(EXIT_FAILURE);
        }
        strncpy(current->value, &lex->rules_list.list[i].pattern[start], escape_length);
        current->value[escape_length] = '\0';
    }
}

void	optional_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	size_t start = *j;
	while (lex->rules_list.list[i].pattern[*j] == '?' && lex->rules_list.list[i].pattern[*j] != '\0')
		(*j)++;
	if (*j > start)
	{
		current->value = realloc(current->value, *j - start + 1);
		if (!current->value)
		{
			perror("Memory allocation failed for optional token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start);
		current->value[*j - start] = '\0';
	}
	else
	{
		current->value = strdup("?");
		if (!current->value)
		{
			perror("Memory allocation failed for optional token value");
			exit(EXIT_FAILURE);
		}
	}
}

void	left_paren_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	(void)i;
	current->value = strdup("(");
	if (!current->value)
	{
		perror("Memory allocation failed for left parenthesis token value");
		exit(EXIT_FAILURE);
	}
	current->value[0] = lex->rules_list.list[i].pattern[*j];
	current->value[1] = '\0';
}

void	right_paren_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	(void)i;
	(void)j;
	current->value = strdup(")");
	if (!current->value)
	{
		perror("Memory allocation failed for right parenthesis token value");
		exit(EXIT_FAILURE);
	}
	current->value[0] = lex->rules_list.list[i].pattern[*j];
	current->value[1] = '\0';
}

void	quantifier_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	size_t start = *j;
	while (lex->rules_list.list[i].pattern[*j] == '{' || lex->rules_list.list[i].pattern[*j] == '}' || 
		   lex->rules_list.list[i].pattern[*j] == ',' || isdigit(lex->rules_list.list[i].pattern[*j]))
	{
		(*j)++;
	}
	if (*j > start)
	{
		current->value = realloc(current->value, *j - start + 1);
		if (!current->value)
		{
			perror("Memory allocation failed for quantifier token value");
			exit(EXIT_FAILURE);
		}
		strncpy(current->value, &lex->rules_list.list[i].pattern[start], *j - start);
		current->value[*j - start] = '\0';
	}
	else
	{
		current->value = strdup("{}");
		if (!current->value)
		{
			perror("Memory allocation failed for quantifier token value");
			exit(EXIT_FAILURE);
		}
	}
}

void	char_token(t_token *current, t_lex *lex, size_t i, size_t *j)
{
	current->value = malloc(2); // Allocate memory for a single character + null terminator
	if (!current->value)
	{
		perror("Memory allocation failed for char token value");
		exit(EXIT_FAILURE);
	}
	current->value[0] = lex->rules_list.list[i].pattern[*j];
	current->value[1] = '\0';
}
