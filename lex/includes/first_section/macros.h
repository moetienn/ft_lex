#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_macro
{
	char	*name; // Name of the macro
	char	*value; // Value of the macro
}	t_macro;

typedef struct s_macros_list
{
	t_macro	*list; // Array of macros
	size_t	count; // Number of macros
    size_t	capacity; // Capacity of the array
}	t_macros_list;

void	init_macros_list(t_macros_list *macros_list);
void	free_macros_list(t_macros_list *macros_list);
void	add_macro(t_macros_list *macros_list, const char *name, const char *value);

#endif