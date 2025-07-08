#include "../../includes/rules/rules.h"

void    init_rules_list(t_rules_list *rules_list)
{
    if (!rules_list)
        return;
    rules_list->list = NULL;
    rules_list->count = 0;
    rules_list->capacity = 0;
}

void    add_rule(t_rules_list *rules_list, const char *pattern, const char *action)
{
    if (!rules_list || !pattern || !action)
        return;
    if (rules_list->count >= rules_list->capacity)
    {
        size_t new_capacity = rules_list->capacity == 0 ? 1 : rules_list->capacity * 2;
        t_rule *new_list = realloc(rules_list->list, new_capacity * sizeof(t_rule));
        if (!new_list)
            return;
        rules_list->list = new_list;
        rules_list->capacity = new_capacity;
    }
    rules_list->list[rules_list->count].pattern = strdup(pattern);
    rules_list->list[rules_list->count].action = strdup(action);
    rules_list->count++;
}

void    free_rules_list(t_rules_list *rules_list)
{
    if (!rules_list || !rules_list->list)
        return;
    for (size_t i = 0; i < rules_list->count; i++)
    {
        free(rules_list->list[i].pattern);
        free(rules_list->list[i].action);
    }
    free(rules_list->list);
    rules_list->list = NULL;
    rules_list->count = 0;
    rules_list->capacity = 0;
}