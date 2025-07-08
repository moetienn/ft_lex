#include "../../includes/lex.h"

char    *extract_pattern(const char *str, size_t *i)
{
    char    *pattern = NULL;
    size_t  start = *i;

    while (str[*i] && str[*i] != '\n' && str[*i] != ' ' && str[*i] != '\t')
        (*i)++;
    size_t len = *i - start;
    printf("len: %zu\n", len);
    pattern = malloc(len + 1);
    if (!pattern)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    strncpy(pattern, &str[start], len);
    pattern[len] = '\0';
    return (pattern);
}

char    *extract_action(const char *str, size_t *i)
{
    char    *action = NULL;
    size_t  start = *i;

    while (str[*i] && str[*i] != '}')
        (*i)++;
    size_t len = *i + 1 - start;
    action = malloc(len + 1);
    if (!action)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    strncpy(action, &str[start], len);
    action[len] = '\0';
    return (action);
}

void    collect_rules(t_lex *lex)
{
    size_t i = 0;
    char *pattern = NULL;
    char *action = NULL;

    printf("Collecting rules...\n");
    while (lex->parser.second_section[i])
    {
        if (lex->parser.second_section[i] != ' ' && lex->parser.second_section[i] != '\t' && lex->parser.second_section[i] != '\n' && lex->parser.second_section[i] != '{' && lex->parser.second_section[i] != '}')
        {
            pattern = extract_pattern(lex->parser.second_section, &i);
        }
        if (lex->parser.second_section[i] == '{')
        {
            action = extract_action(lex->parser.second_section, &i);
        }
        if (pattern != NULL && action != NULL)
        {
            printf("pattern: %s\n", pattern);
            add_rule(&lex->rules_list, pattern, action);
            free(pattern);
            free(action);
            pattern = NULL;
            action = NULL;
        }
        (i++);
    }
    if (pattern != NULL)
    {
        free(pattern);
    }
    if (action != NULL)
    {
        free(action);
    }
    // printf("Collected %zu rules.\n", lex->rules_list.count);
    for (size_t j = 0; j < lex->rules_list.count; j++)
    {
        t_rule *rule = &lex->rules_list.list[j];
        printf("Rule %zu: %s -> %s\n", j + 1, rule->pattern, rule->action);
    }
}
