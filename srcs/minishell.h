
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
    struct s_env        *next;
}                       t_env;

typedef struct          s_token
{
    char                *word;
    struct s_token      *next;
}                       t_token;

// int		our_echo(char **argv);
int		our_echo(char *argument);
int		our_pwd(void);
void    ft_error(int code);
int our_env(char **env);
int	our_cd(int argc, char *path);
#endif