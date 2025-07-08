#ifndef RULES_H
# define RULES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_rule
{
    char *pattern;
    char *action;
}   t_rule;

typedef struct s_rules_list
{
    t_rule *list;
    size_t count;
    size_t capacity;
}   t_rules_list;

// utils
void    init_rules_list(t_rules_list *rules_list);
void    free_rules_list(t_rules_list *rules_list);
void    add_rule(t_rules_list *rules_list, const char *pattern, const char *action);

#endif