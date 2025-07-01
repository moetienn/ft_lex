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
    t_parser parser;
    
    parser.file = file;
    parser.filename = strdup(filename);
    parse_file(&parser);
    free_parser(&parser);
    // end of processing
}