#include "../../includes/lex.h"

void process_rule(t_lex *lex, char *pattern, char *action)
{
	add_rule(&lex->rules_list, pattern, action);
	free(pattern);
	free(action);
}

void collect_rules(t_lex *lex)
{
	size_t i = 0;
	char *pattern = NULL;
	char *action = NULL;

	while (lex->parser.second_section[i])
	{
		if (pattern == NULL && !is_whitespace(lex->parser.second_section[i]) && lex->parser.second_section[i] != '{' && lex->parser.second_section[i] != '}')
			pattern = extract_pattern(lex->parser.second_section, &i);
		else if (pattern == NULL && lex->parser.second_section[i] == '{')
			pattern = pattern_to_macro(lex->parser.second_section, &i, &lex->macros_list);
		if (lex->parser.second_section[i] == '{')
			action = extract_action(lex->parser.second_section, &i);
		if (pattern != NULL && action != NULL)
		{
			process_rule(lex, pattern, action);
			pattern = NULL;
			action = NULL;
		}
		i++;
	}
	if (pattern != NULL)
		free(pattern);
	if (action != NULL)
		free(action);
	for (size_t j = 0; j < lex->rules_list.count; j++)
	{
		t_rule *rule = &lex->rules_list.list[j];
		printf("Rule %zu: %s -> %s\n", j + 1, rule->pattern, rule->action);
	}
}
