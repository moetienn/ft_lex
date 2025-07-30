# include "../../includes/lex.h"

bool    is_atom_quantif_rparen(t_token *token)
{
    if (!token)
        return (false);
    if (token->type == TOKEN_CHAR)
        return (true);
    else if (token->type == TOKEN_CLASS)
        return (true);
    else if (token->type == TOKEN_QUOTE)
        return (true);
    else if (token->type == TOKEN_ESCAPE)
        return (true);
    else if (token->type == TOKEN_KLEENE_STAR)
        return (true);
    else if (token->type == TOKEN_PLUS)
        return (true);
    else if (token->type == TOKEN_OPTIONAL)
        return (true);
    else if (token->type == TOKEN_RPAREN)
        return (true);
    else if (token->type == TOKEN_QUANTIFIER)
        return (true);
    return (false);
}

bool    is_atom_opengroup(t_token *token)
{
    if (!token)
        return (false);
    if (token->type == TOKEN_CHAR)
        return (true);
    else if (token->type == TOKEN_CLASS)
        return (true);
    else if (token->type == TOKEN_QUOTE)
        return (true);
    else if (token->type == TOKEN_ESCAPE)
        return (true);
    else if (token->type == TOKEN_LPAREN)
        return (true);
    return (false);
}

void    add_concat_tokens(t_lex *lex)
{
    for (size_t i = 0; i < lex->rules_list.count; i++)
    {
        t_token *current = lex->token_list[i];
        while (current && current->next)
        {
            if (is_atom_quantif_rparen(current) && is_atom_opengroup(current->next))
            {
                t_token *concat_token = malloc(sizeof(t_token));
                if (!concat_token)
                {
                    perror("Memory allocation failed for concatenation token");
                    exit(EXIT_FAILURE);
                }
                concat_token->type = TOKEN_CONCAT;
                concat_token->value = strdup("·");
                concat_token->next = current->next;
                current->next = concat_token;
            }
            current = current->next;
        }
    }
}
