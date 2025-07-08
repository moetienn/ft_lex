#include "../../includes/declaration/macros.h"

void    free_macros_list(t_macros_list *macros_list)
{
    for (size_t i = 0; i < macros_list->count; i++)
    {
        free(macros_list->list[i].name);
        free(macros_list->list[i].value);
    }
    free(macros_list->list);
}

void	add_macro(t_macros_list *macros_list, const char *name, const char *value)
{
    if (macros_list->count >= macros_list->capacity)
    {
        size_t new_capacity = macros_list->capacity == 0 ? 1 : macros_list->capacity * 2;
        t_macro *new_list = realloc(macros_list->list, new_capacity * sizeof(t_macro));
        if (!new_list)
        {
            perror("Failed to allocate memory for macros list");
            exit(EXIT_FAILURE);
        }
        macros_list->list = new_list;
        macros_list->capacity = new_capacity;
    }
    macros_list->list[macros_list->count].name = strdup(name);
    macros_list->list[macros_list->count].value = strdup(value);
    macros_list->count++;
}

void	init_macros_list(t_macros_list *macros_list)
{
    macros_list->list = NULL;
    macros_list->count = 0;
    macros_list->capacity = 0;
}