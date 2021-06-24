#ifndef MINISHELL_H
# define MINISHELL_H


// # include <curses.h>
# include <signal.h>
// # include <term.h>
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
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct          s_env
{
    char                *key;
	char				*value;
	struct s_env		*next;
}                       t_env;

typedef struct          s_store
{
    char                *word;
    struct s_store      *next;
}                       t_store;


typedef	struct s_minishell
{
	// struct termios		term;
	// struct termios		ret_term;
	int					fork_status;
	int					exit_code;
	// int					win_col;
	// int					win_row;
	// int					col;
	// int					row;
	// char				*cm;
	// char				*ce;
	// char				*dc;

	int					sig_flag_int;
	int					sig_flag_quit;

}						t_shell;

t_shell                 t_sh;

void	our_sig_proc(int sig);
int 	our_unset(t_env *env, t_env *export, t_store *token);
int		exec_bin(t_store *token, t_env *env_value, char **env);
int		our_echo(char *argument);
int		our_pwd(void);
int 	our_env(t_env *env);
int     our_cd(int argc, char *path, t_env *env, t_env *export);
int		our_export(t_env *env, t_env *export, t_store *token);

int		free_exit(t_store *store, t_env *env, int code);

char 	*get_key(char *token);
char 	*get_value(char *token);
t_env 	*add_node_env(t_env *env, char *key, char *value);
char 	*single_quotes(char *str, int *i);
char 	*double_quotes(char *str, int *i, t_env *env);
char 	*process_dollar(char *str, int *i, t_env *env);
int check_export_argument(char *arg);
void    ft_error(int code);


#endif