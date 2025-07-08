#include "../../includes/lex.h"
#include "../../includes/nfa/nfa.h"

struct s_token	*token_new(t_token_type type, const char *value)
{
    t_token	*new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    new_token->type = type;
    if (value)
    {
        new_token->value = strdup(value);
        if (!new_token->value)
        {
            free(new_token);
            perror("Memory allocation failed");
            return NULL;
        }
    }
    else
        new_token->value = NULL;
    new_token->next = NULL;
    return new_token;
}

void	free_token(t_token *head)
{
    t_token	*tmp;

    while (head)
    {
        tmp = head;
        head = head->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
    }
}

void	add_token(t_token **head, t_token *tok)
{
    if (!head || !tok)
        return;
    if (!*head)
    {
        *head = tok;
    }
    else
    {
        t_token	*current = *head;
        while (current->next)
            current = current->next;
        current->next = tok;
    }
}

void	init_token(t_token **token)
{
	if (!token)
		return;
    *token = malloc(sizeof(t_token));
	if (!*token)
		return;
	(*token)->type = TOKEN_END;
	(*token)->value = NULL;
	(*token)->next = NULL;
}