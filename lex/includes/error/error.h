#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_error
{
	char    **file_lines;
	char    *message;
}   t_error;

void    initialize_error(t_error *error);
void    free_error(t_error *error);
void	add_to_error(t_error *error, char *line, size_t len);

#endif