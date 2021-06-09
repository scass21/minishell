#include "minishell.h"

void ft_error(int code)
{
    if (code == 1)
        printf("Error: malloc.\n");
    exit(EXIT_FAILURE);
}