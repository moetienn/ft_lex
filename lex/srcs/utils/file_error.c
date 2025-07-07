#include "../../includes/lex.h"

int	open_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		return -1;
	}
	fclose(file);
	return 0;
}

void	file_error(const char *filename)
{
	if (open_file(filename) == -1)
	{
		fprintf(stderr, "Error: Could not open file '%s'\n", filename);
		exit(1);
	}
	if (strstr(filename, ".l") == NULL)
	{
		fprintf(stderr, "Error: File '%s' is not a valid .l file\n", filename);
		exit(1);
	}
}
