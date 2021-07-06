#ifndef MINISHELL_H
# define MINISHELL_H


// # include <curses.h>
# include <signal.h>
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
	struct s_store      *previous;
}                       t_store;




typedef	struct s_minishell
{
	// struct termios		term;
	// struct termios		ret_term;
	int					fork_status;
	int					exit_code;
	int					fd;
	int					fd2;
	int					pipe_flag;
	// int					win_col;
	// int					win_row;
	// int					col;
	// int					row;
	// char				*cm;
	// char				*ce;
	// char				*dc;

	// int					sig_flag_int;
	// int					sig_flag_quit;

}						t_shell;

typedef struct s_all
{
	t_env *env;
	t_env *export;
	t_store *token;
	t_shell *shell;
}				t_all;

t_shell                 t_sh;

void	our_sig_proc(int sig);
int our_unset(t_env *env, t_env *export, char **argv, int count);
int		exec_bin(char **argv, t_env *env_value, char **env);
int		our_echo(char **argv);
int		our_pwd(char **argv);
int our_env(t_env *env, char **argv, int count);
int	our_cd(int argc, char **argv, t_env *env, t_env *export);
int	our_export(int count, t_env *env, t_env * export, char **argv);
void print_error(char *str);

char *process_redirect(char *str, t_env *env, t_store *store);
int our_redirect(char *word, t_env *env, t_store *token);

int		free_exit(t_store *store, t_env *env, int code);
void rl_replace_line();

char 	*get_key(char *token);
char 	*get_value(char *token);
t_env 	*add_node_env(t_env *env, char *key, char *value);
char 	*single_quotes(char *str, int *i);
char 	*double_quotes(char *str, int *i, t_env *env);
char 	*process_dollar(char *str, int *i, t_env *env);
int check_export_argument(char *arg);
void    ft_error(int code);

t_store *add_node_token(t_store *token, char *str, int i);
int execute_command(t_env *env, t_env * export, t_store *token, char **envp);
char *process_value(char *val, t_env *env);
char *process_pipe(char *str, t_env *env, t_store *token);
int if_pipe(t_store *token, char **envp, t_env *export, t_env *env);

int     command_handler(t_env *env, t_env *export, t_store *token, char **envp);
int pipe_proc(char *str, t_env *env, char **envp, t_env *export, t_store *token);


#endif


//gcc -g -Wall -Wextra lexer.c cd.c echo.c env.c error.c exec.c exit.c export.c pwd.c signal.c unset.c -lreadline ../libft/libft.a -L/Users/lkasandr/.brew/Cellar/readline/8.1/lib/ redirect.c pipe.c 