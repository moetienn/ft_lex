#ifndef LEX_H
# define LEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

void    file_error(const char *filename);
void	ft_lex(const char *filename);

#endif