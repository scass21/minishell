# include "minishell.h"

char	**make_arg(t_store *token, char **arg)
{
	int		i;
	int		j;
	t_store *temp;
    
	temp = token;
    i = 0;
	while (temp != NULL)
	{
		i++;
		temp = temp->next;
	}
	temp = token;
	j = 0;
	while (j < i)
	{
		arg[j] = temp->word;
		temp = temp->next;
		j++;
	}

	return (arg);
}

int		exec_bin(t_store *token)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**arg;

	arg = make_arg(token, arg);
	path = ft_strdup("/usr/bin/");
	path = ft_strjoin(path, arg[0]);
	pid = fork();
	signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
	if (pid == 0)
	{
		t_sh.fork_status = 1;
		if (execve(path, arg, NULL) < 0)
		{
			printf("%s\n", strerror(errno));
			// exit(1);
		}
		// exit(0);
	}
	else if (pid < 0)
	{
		printf("%s\n", strerror(errno));
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			t_sh.exit_code = WEXITSTATUS(status);
	}
	return 1;
}


