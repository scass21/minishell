#include "minishell.h"

void ft_error(int code)
{
    if (code == 1)
        printf("Error: malloc.\n");
    if (code == 2)
        printf("Can't read file\n");
    exit(EXIT_FAILURE);
}