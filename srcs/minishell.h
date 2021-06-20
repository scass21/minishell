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
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct          s_store
{
    char                *word;
    struct s_store      *next;
}                       t_store;


typedef	struct s_minishell
{
	struct termios		term;
	struct termios		ret_term;
	int					fork_status;
	int					exit_code;
	int					win_col;
	int					win_row;
	int					col;
	int					row;
	char				*cm;
	char				*ce;
	char				*dc;

}						t_shell;

t_shell                 t_sh;

void	our_sig_proc(int sig);
int		exec_bin(t_store *token);
int		our_echo(char *argument);
int		our_pwd(void);
void    ft_error(int code);
int 	our_env(t_store *env);
int     our_cd(int argc, char *path);
int		our_export(t_store *env, t_store * export, t_store *token);
char 	*single_quotes(char *str, int *i);
char 	*double_quotes(char *str, int *i, t_store *env);
char 	*process_dollar(char *str, int *i, t_store *env);
void 	init_struct_store(t_store *token);
t_store *add_node_env(t_store *env, char *str);
int our_unset(t_store *env, t_store *export, t_store *token);


#endif