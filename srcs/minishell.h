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
# include <termios.h>
# include <signal.h>
// # include <wait.h>
# include <sys/ioctl.h>
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

typedef	struct          s_minishell
{
    t_env               *env;
	struct termios		term;
	struct termios		ret_term;
	char				*path;
	char				*str;
	int					*fork_status;
	int					exit_code;
	int					win_col;
	int					win_row;
	int					col;
	int					row;
	char				*cm;
	char				*ce;
	char				*dc;

}						t_shell;

t_shell					t_sh;

int		our_echo(char *argument);
int		our_pwd(void);
void    ft_error(int code);
int     our_env(char **env);
int     our_cd(int argc, char *path);

#endif