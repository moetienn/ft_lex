#include "../../includes/lex.h"

void	process_rule(t_lex *lex, char *pattern, char *action)
{
	add_rule(&lex->rules_list, pattern, action);
	free(pattern);
	free(action);
}

int	check_valid_macro(const char *macro)
{
	if (macro == NULL || strlen(macro) == 0)
	{
		fprintf(stderr, "Error: Invalid macro pattern\n");
		return (-1);
	}
	for (size_t i = 0; i < strlen(macro); i++)
	{
		if (macro[i] == ']')
			return (0);
	}
	return (-1);
}

void	free_error_class(t_lex *lex)
{
	fprintf(stderr, "%s:%d: Bad character class\n", lex->parser.filename, lex->parser.error_line);
	fprintf(stderr, "%s:%d: unrecognized rule\n", lex->parser.filename, lex->parser.error_line);
	free(lex->parser.second_section);
	free_macros_list(&lex->macros_list);
	free_rules_list(&lex->rules_list);
	free(lex->declaration_code);
	free(lex->parser.first_section);
	free(lex->user_code);
	free(lex->parser.filename);
	free(lex->parser.line);
	free(lex->parser.third_section);
	free_error(&lex->parser.error);
	fclose(lex->parser.file);
	exit(EXIT_FAILURE);
}

void	collect_rules(t_lex *lex)
{
	size_t i = 0;
	char *pattern = NULL;
	char *action = NULL;

	while (lex->parser.second_section[i])
	{
		if (pattern == NULL && !is_whitespace(lex->parser.second_section[i]) && lex->parser.second_section[i] != '{' && lex->parser.second_section[i] != '}')
			pattern = extract_pattern(lex->parser.second_section, &i);
		else if (pattern == NULL && lex->parser.second_section[i] == '{')
		{
			pattern = pattern_to_macro(lex->parser.second_section, &i, &lex->macros_list);
			if (check_valid_macro(pattern) == -1)
			{
				free(pattern);
				pattern = NULL;
				free_error_class(lex);
			}
		}
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
}
