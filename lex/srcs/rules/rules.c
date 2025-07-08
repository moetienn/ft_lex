#include "../../includes/lex.h"

char    *extract_pattern(const char *str, size_t *i)
{
    char    *pattern = NULL;
    size_t  start = *i;

    while (str[*i] && str[*i] != '\n' && str[*i] != ' ' && str[*i] != '\t')
        (*i)++;
    size_t len = *i - start;
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

char    *replace_tmp_with_macro(const char *tmp, t_macros_list *macros_list)
{
    for (size_t i = 0; i < macros_list->count; i++)
    {
        if (strcmp(macros_list->list[i].name, tmp) == 0)
            return (strdup(macros_list->list[i].value));
    }
    return (strdup(tmp));
}

char *append_macro(const char *str, size_t *i, char *macro, size_t start)
{
    while (str[*i] && str[*i] != '\n' && str[*i] != ' ' && str[*i] != '\t')
        (*i)++;
    size_t rest_len = *i - start;
    macro = realloc(macro, strlen(macro) + rest_len + 1);
    if (!macro)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    strncat(macro, &str[start], rest_len);
    macro[strlen(macro) + rest_len] = '\0';
    return macro;
}

char *pattern_to_macro(const char *str, size_t *i, t_macros_list *macros_list)
{
    char    *tmp = NULL;
    char    *macro = NULL;
    size_t  start = *i + 1;

    while (str[*i] && str[*i] != '}')
        (*i)++;
    size_t len = *i - start;
    tmp = malloc(len + 1);
    if (!tmp)
    {
        perror("Memory allocation failed");
        return NULL;
    }
    strncpy(tmp, &str[start], len);
    tmp[len] = '\0';
    macro = replace_tmp_with_macro(tmp, macros_list);
    if (str[*i] == '}' && str[*i + 1] != '\n' && str[*i + 1] != ' ' && str[*i + 1] != '\t' && str[*i + 1] != '\0')
    {
        start = *i + 1;
        macro = append_macro(str, i, macro, start);
        if (!macro)
            return (free(tmp), NULL);
    }
    return (free(tmp), macro);
}

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
        if (pattern == NULL && lex->parser.second_section[i] != ' ' && lex->parser.second_section[i] != '\t' && lex->parser.second_section[i] != '\n' && lex->parser.second_section[i] != '{' && lex->parser.second_section[i] != '}')
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
