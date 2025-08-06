#include "../../includes/error/error.h"

void	initialize_error(t_error *error)
{
	if (!error)
		return;
	error->file_lines = NULL;
	error->message = NULL;
}

void	free_error(t_error *error)
{
	if (!error)
		return;
	for (size_t i = 0; error->file_lines && error->file_lines[i]; i++)
	{
		free(error->file_lines[i]);
		error->file_lines[i] = NULL;
	}
	free(error->file_lines);
	error->file_lines = NULL;
	free(error->message);
	error->message = NULL;
}

void	add_to_error(t_error *error, char *line, size_t len)
{
	if (error->file_lines == NULL)
	{
		error->file_lines = malloc(sizeof(char *));
		error->file_lines[0] = NULL;
	}
	size_t count = 0;
	while (error->file_lines[count])
		count++;
	error->file_lines = realloc(error->file_lines, sizeof(char *) * (count + 2));
	error->file_lines[count] = strndup(line, len);
	error->file_lines[count + 1] = NULL;
}
