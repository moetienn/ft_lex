#ifndef PARSER_H
# define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <unistd.h>

typedef enum e_section
{
	SEC_DECLARATIONS,
	SEC_RULES,
	SEC_USER_CODE
}   t_section;

typedef struct s_parser
{
	FILE		*file;
	char		*filename;
	char		*buffer;
	char		*line;
	size_t		len;
	char		*first_section;
	char		*second_section;
	char		*third_section;
	int 		delimiter_count;
	t_section	current_section;
}   t_parser;

// UTILS.C

int		check_parser(t_parser *parser);
void	free_parser(t_parser *parser);

int		init_parser(t_parser *parser);

void	parse_file(t_parser *parser);

#endif