# include "minishell.h"

// int     command_handler_pip(char **argv, t_env *env, t_env *export, t_store *token, char **envp)
// {
//     int count;

// 	count = count_argument(argv);
// 	if (argv)
//     {
        
//         if (ft_strncmp(argv[0], "echo", 4) == 0)
//             our_echo(argv);
//         else if (ft_strncmp(argv[0], "pwd", 3) == 0)
//             our_pwd(argv);
//         else if (ft_strncmp(argv[0], "cd", 2) == 0)
//             our_cd(count, argv, env, export);
//         else if (ft_strncmp(argv[0], "export", 6) == 0)
//             our_export(count, env, export, argv);
//         else if (ft_strncmp(argv[0], "unset", 5) == 0)
//             our_unset(env, export, argv, count);
//         else if (ft_strncmp(argv[0], "env", 3) == 0)
//              our_env(env, argv, count);
//         else if (ft_strcmp(argv[0],  "$?") == 0)
//         {
//             ft_putstr_fd("minishell: ",2);
//             ft_putnbr_fd(t_sh.exit_code , 2);
//             ft_putstr_fd(": command not found\n",2);
//             return (0);
//         }
//         else if (ft_strcmp(argv[0], "exit") == 0)
//             free_exit(token, env, 0);
//         else
//             exec_bin(argv, env, envp);
        
//     }
//     t_sh.exit_code = 0;
//     return (0);

// }



int pipe_proc(char *str, t_env *env, char **envp, t_env *export, t_store *token)

{
	int		fd[2];
	pid_t	pid;
	int		status;
	int 	old_fd_in;
	int 	old_fd_out;
	t_store *tmp;

	tmp = token;
	// while(tmp)
	// {
	// 	printf("%s\n", tmp->word);
	// 	tmp = tmp->next;
	// }
	tmp = token;
	old_fd_in = dup(0);
	old_fd_out = dup(1);		// сохраняем значения фд, чтобы вернуть на последнем пайпе
	while (tmp)
	{
		if(pipe(fd) == -1)
			return (0);
		pid = fork();
		// if (pid < 0)
		// {
		// 	ft_putstr_fd(strerror(errno), 2);
		// 	write(2, "\n", 1);
		// 	exit(1);
		// }
		if (pid == 0)
		{
			if(tmp->next)
				dup2(fd[1], 1);
			if (tmp->previous)
				dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			// command_handler_pip(argv, env, export, token, envp);
			// exec_bin(argv, env_value, envp);
			command_handler(env, export, tmp, envp);
			exit(0);
		}
		close(fd[0]);
		close(fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			t_sh.exit_code = WEXITSTATUS(status);
		if (tmp->next == 0)
			close(fd[0]);
		close(fd[1]);
		
		tmp = tmp->next;
	}
	
	dup2(old_fd_in, 0);
	dup2(old_fd_out, 1);
	close(old_fd_in);
	close(old_fd_out);
	
	return (0);
	
}


// char 	**argv;
	
	// int i = 0;
	// while (str[i])
	// {
	// 	if (str[i] == '|')
	// 		str[i] = ' ';
	// 	i++;
	// }
	// // printf("%s\n", str);
	// // if (ft_strncmp(str, "| ", 2) == 0)
	// // 	str = ft_substr(str, 2, (ft_strlen(str) - 2));
	// argv = split_token_word(str);
	// // int i = 0;
	// while(argv[i])
	// {
	// 	if (ft_strchr(argv[i], '|'))
	// 		argv[i] = "";
	// 	i++;
	// }