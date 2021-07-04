# include "minishell.h"

// void	fork_for_comm(t_store *token, char **envp, t_env *env, t_env *export)
// {
	
// 	if (pid < 0)
// 	{
// 		printf("%s\n", strerror(errno));
// 		exit(1);
// 	}
// 	else if (pid == 0)
// 	{
// 		if (token->next == 0)
// 			dup2(t_sh.previous->our_pip[0], 0);
// 		else
// 		{
// 			dup2(t_sh.previous->our_pip[0], 0);
// 			dup2(t_sh.our_pip[1], 1);
// 		}
// 		if (t_sh.fd_in != 0)
// 			dup2(t_sh.fd_in, 0);
// 		if (t_sh.fd_out != 1)
// 			dup2(t_sh.fd_out, 1);
// 		execute_command(env, export, token, envp);
// 		exit(0);
// 	}
// 	else
// 	{
// 		close(t_sh.our_pip[1]);
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			t_sh.exit_code = WEXITSTATUS(status);
// 	}
	
// }



int if_pipe(t_store *token, char **envp, t_env *export, t_env *env)
{
	// char **commands;
	t_store *tmp;
	// int		stin;
	// int		stout;
	int		f_d[2];
	pid_t	pid;
	int		status;
	char	*str;

	if (!token)
		return (0);	
	
	else
		tmp = token;
	while(tmp)
	{
		printf("\ntoken in pipe %s\n", tmp->word);
		tmp = tmp->next;
	}

	tmp = token;
	// while (tmp)
	// {
		pipe(f_d);
		

		pid = fork();
		signal(SIGINT, our_sig_proc);
		signal(SIGQUIT, our_sig_proc);
		if (pid == 0)
		{
			dup2(f_d[1], 1);
			close(f_d[0]);
			
			execute_command(env, export, tmp, envp);
			close(f_d[1]);
			// t_sh.pipe_flag = 0;
			exit(0);
		}
		else /*if (pid != 0)*/
		{
			dup2(f_d[0], 0);
			close(f_d[1]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				t_sh.exit_code = WEXITSTATUS(status);
			close(f_d[0]);
		}	
		// tmp = tmp->next;
	// dup2(t_sh.fd2, 0);
	// dup2(t_sh.fd, 1);
	// close(t_sh.fd);
	// close(t_sh.fd2);	
	// if (t_sh.str)
	// {
	// 	// if (tmp)
	// 	// 	free_struct_store(tmp);
	// 	str = ft_strdup(t_sh.str);
	// 	// t_sh.str =  (char *)ft_calloc(ft_strlen(str), sizeof(char *));
	// 	t_sh.str = NULL;
	// 	t_sh.pipe_flag = 0;
	// 	parser(str, env, tmp, export, envp);
	// 	// if (t_sh.pipe_flag == 0)
	// 	// 	execute_command(env, export, token, envp);
		
	// }
	// else
	// }
		return (0);
	
}