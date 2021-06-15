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
#include <termios.h>
#include <signal.h>
#include <wait.h>
# include <sys/ioctl.h>

typedef struct s_list_env
{
	char				**env_copy;
	char				*pwd;
	char				*oldpwd;
	struct s_list_env	*next;
}						t_list_env;

typedef	struct s_minishell
{
	t_list_env			*t_env;
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


// built
int		our_echo(char **argv);
int		our_pwd(void);
// int		our_cd(char *path);
int		our_env(char **env);

#endif