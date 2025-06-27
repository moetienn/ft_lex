#include "../includes/lex.h"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        fprintf(stderr, "Usage: %s <input>\n", av[0]);
        return 1;
    }
    return 0;
}