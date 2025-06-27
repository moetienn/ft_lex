#ifndef PARSER_H
# define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	t_section	current_section;
}   t_parser;

#endif