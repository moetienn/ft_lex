#include "../../includes/lex.h"
#include "../../includes/nfa/nfa.h"

t_token_type get_token_type(char c)
{
	if (c == '[')
		return TOKEN_CLASS;
	else if (c == '\"')
		return TOKEN_QUOTE;
    else if (c == '|')
        return TOKEN_ALTERNATION;
    else if (c == '*')
        return TOKEN_KLEENE_STAR;
    else if (c == '+')
        return TOKEN_PLUS;
    else if (c == '?')
        return TOKEN_OPTIONAL;
    else if (c == '(')
        return TOKEN_LPAREN;
    else if (c == ')')
        return TOKEN_RPAREN;
    else
        return TOKEN_CHAR; // Default to literal character
}

void    free_token_list(t_token **token_list)
{
	if (!token_list)
		return;

	for (size_t i = 0; token_list[i]; i++)
	{
		free(token_list[i]->value);
		free(token_list[i]);
	}
	free(token_list);
}

void	init_token_list(t_token ***token_list, size_t count)
{
	if (!token_list)
		return;

	printf("count : %zu\n", count);
	*token_list = malloc(sizeof(t_token *) * count);
	if (!*token_list)
	{
		perror("Failed to allocate memory for token list");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < count; i++)
	{
		(*token_list)[i] = NULL; // Initialize each token list entry to NULL
	}
}

void	add_next_token(t_token **current, t_token_type type, const char *value)
{
	(void) value; // If value is not used, we can ignore it
    (*current)->next = malloc(sizeof(t_token));
    if (!(*current)->next)
    {
        perror("Memory allocation failed for next token");
        exit(EXIT_FAILURE);
    }
    (*current)->next->type = type;
    (*current)->next->value = malloc(1);
    (*current)->next->next = NULL;
    *current = (*current)->next; // Move the current pointer to the newly created token
}

void initialize_first_token(t_token **token_list, t_token **current, t_token_type type)
{
    *token_list = malloc(sizeof(t_token));
    if (!*token_list)
    {
        perror("Failed to allocate memory for token");
        exit(EXIT_FAILURE);
    }
    (*token_list)->type = type;
    (*token_list)->value = NULL;
    (*token_list)->next = NULL;
    *current = *token_list;
}
