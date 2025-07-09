#include "../../includes/lex.h"

void	handle_token_type(t_token *current, t_lex *lex, size_t i, size_t *j, t_token_type type)
{
    if (type == TOKEN_CLASS)
        class_token(current, lex, i, j);
    else if (type == TOKEN_PLUS)
        plus_token(current, lex, i, j);
    else if (type == TOKEN_QUOTE)
        quote_token(current, lex, i, j);
    else if (type == TOKEN_ALTERNATION)
        alternation_token(current, lex, i, j);
	else if (type == TOKEN_KLEENE_STAR)
		kleene_star_token(current, lex, i, j);
}

void	process_pattern_tokens(t_lex *lex, size_t i, size_t pattern_length, t_token **current)
{
    for (size_t j = 0; j < pattern_length; j++)
    {
        char c = lex->rules_list.list[i].pattern[j];
        t_token_type type = get_token_type(c);

        if (!(*current))
            initialize_first_token(&lex->token_list[i], current, type);
        else
            add_next_token(current, type, NULL);
        handle_token_type(*current, lex, i, &j, type);
    }
}

void	tokenize_patterns(t_lex *lex)
{
	printf("Tokenizing patterns...\n");
	for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		if (!lex->rules_list.list[i].pattern) // Ensure pattern is valid
		{
			fprintf(stderr, "Error: Pattern for rule %zu is NULL\n", i);
			continue;
		}

		t_token *current = NULL;
		size_t pattern_length = strlen(lex->rules_list.list[i].pattern); // Get pattern length

		process_pattern_tokens(lex, i, pattern_length, &current);
	}
	printf("Tokenization complete.\n");

	// Print tokens for debugging
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
