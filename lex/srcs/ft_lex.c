#include "../includes/lex.h"

void    ft_lex(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return;
    }
    // Process the file...

    
    // 1st step: identify sections
    // -> 1ST section
    // -> %{ %} -> C code section to add on the top of the yy.c file
    // -> then definitions section
    // -> 2ND section
    // -> rules section
    // -> 3RD section
    // -> user code section

    // end of processing
    fclose(file);
}