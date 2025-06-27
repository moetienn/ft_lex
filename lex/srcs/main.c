#include "../includes/lex.h"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        fprintf(stderr, "Usage: %s <file.l>\n", av[0]);
        return 1;
    }
    file_error(av[1]);
    printf("File '%s' is valid and opened successfully.\n", av[1]);
    ft_lex(av[1]);
    return 0;
}