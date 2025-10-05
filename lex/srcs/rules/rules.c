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
		// fprintf(stderr, "Error: Invalid macro pattern\n");
		return (2);
	}
	for (size_t i = 0; i < strlen(macro); i++)
	{
		if (macro[i] == ']')
			return (0);
	}
	if (macro[0] != '[' && macro[strlen(macro) - 1] != ']')
		return (0);
	return (-1);
}

int	check_if_correct_line(t_lex *lex, size_t i)
{
	int j = 0;

	while (lex->parser.error.file_lines[i][j])
	{
		if (lex->parser.error.file_lines[i][j])
		{
			if (lex->parser.error.file_lines[i][j] == '{')
				return (0); 
		}
		j++;
	}
	return (1);
}

int	search_correct_line(t_lex *lex, const char *error_name)
{
	for (size_t i = 0; lex->parser.error.file_lines[i] != NULL; i++)
	{
		if (strstr(lex->parser.error.file_lines[i], error_name) != NULL)
		{
			printf("line to check : %s for macro name %s\n", lex->parser.error.file_lines[i], error_name);
			if (check_if_correct_line(lex, i) == 0)
				return (i + 1);
			else
				continue;
		}
	}
	return (lex->parser.error_line);
}

void	free_error_class(t_lex *lex)
{
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
			pattern = pattern_to_macro(lex->parser.second_section, &i, &lex->macros_list, lex);
			printf("Pattern after macro: %s\n", pattern);
			if (check_valid_macro(pattern) != 0)
			{
				if(check_valid_macro(pattern) == -1)
				{
					fprintf(stderr, "%s:%d: bad character class\n", lex->parser.filename, search_correct_line(lex, pattern));
					fprintf(stderr, "%s:%d: unrecognized rule\n", lex->parser.filename, search_correct_line(lex, pattern));
				}
				else if (check_valid_macro(pattern) == 2)
				{
					fprintf(stderr, "%s:%d: undefined definition\n", lex->parser.filename, search_correct_line(lex, pattern));
					fprintf(stderr, "%s:%d: unrecognized rule\n", lex->parser.filename, search_correct_line(lex, pattern));
				}
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
