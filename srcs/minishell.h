
#ifndef MINISHELL_H
# define MINISHELL_H

# include <curses.h>
# include <term.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <stdio.h>
# include "../libft/libft.h"
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>


typedef struct          s_env
{
    char                *key;
    char                *value;
    struct s_env        *next;
}                       t_env;


// int		our_echo(char **argv);
int		our_echo(char *argument);
int		our_pwd(void);
void ft_error(int code);
#endif