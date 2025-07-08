#include "../../includes/lex.h"

void	tokenize_patterns(t_lex *lex)
{
    (void)lex;
    // print rules for debugging
    printf("Tokenizing patterns...\n");
    for (size_t i = 0; i < lex->rules_list.count; i++)
	{
		printf("Rule %zu: Pattern: %s\n", i, lex->rules_list.list[i].pattern);
        for (size_t j = 0; lex->rules_list.list[i].pattern[j] != '\0'; j++)
        {
            if (lex->rules_list.list[i].pattern[j] == '[')
            {
                // Handle opening bracket
            }
        }
	}
}