# include "minishell.h"

void	fork_for_comm(t_store *token, char **envp, t_env *env, t_env *export)
{
	pid_t	pid;
	int		status;
	int pip[2];

	pid = fork();
	if (pid < 0)
	{
		printf("%s\n", strerror(errno));
		exit(1);
	}
	else if (pid == 0)
	{
		if (token->next == 0)
			dup2(pip[0], 0);
		else
		{
			dup2(pip[0], 0);
			dup2(pip[1], 1);
		}
		if (t_sh.fd2 != 0)
			dup2(t_sh.fd2, 0);
		if (t_sh.fd != 1)
			dup2(t_sh.fd, 1);
		execute_command(env, export, token, envp);
		exit(0);
	}
	else
	{
		close(pip[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			t_sh.exit_code = WEXITSTATUS(status);
	}
	
}



int if_pipe(t_store *token, char **envp, t_env *export, t_env *env)
{
	// char **commands;
	t_store *tmp;
	int		stin;
	int		stout;
	char	*str;

	int pip[2];
	
	str = NULL;
	if (!token)
		return (0);	
	
	else
		tmp = token;
	while (tmp)
	{
		printf("%s\n", token->word);
		pipe(pip);
		stin = dup(0);
		stout = dup(1);
		fork_for_comm(tmp, envp, env, export);
		
		
		
		if(tmp->previous != 0)
			close(pip[0]);
		close(pip[1]);
		if (tmp->next == 0)
			close(pip[0]);
		if (t_sh.fd2 != 0)
			close(t_sh.fd2);
		if (t_sh.fd != 1)
			close(t_sh.fd);
		dup2(stin, 0);
		dup2(stout, 1);
		close(stin);
		close(stout);
		
		tmp = tmp->next;
		
	}
	// if (t_sh.str)
	// {
	// 	// free_struct_store(tmp);
	// 	// str = ft_strdup(t_sh.str);
	// 	// t_sh.str =  (char *)ft_calloc(ft_strlen(str), sizeof(char *));
	// 	t_sh.pipe_flag = 0;
	// 	// parser(str, env, tmp, export, envp);
		
	// 	parser(t_sh.str, env, token, export, envp);
	// 	// if (t_sh.pipe_flag == 0)
	// 	// 	execute_command(env, export, token, envp);
	// 	// free_struct_store(tmp);
	// }
	// t_sh.pipe_flag = 0;
	return (0);
	
}