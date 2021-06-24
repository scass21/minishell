# include "minishell.h"

char	**make_arg(t_store *token, char **arg)
{
	int     i;
    int     j;
    int     len;
    t_store *temp;
	
    temp = token;
    i = 0;
    j = 0;
    len = 0;
    while (temp != NULL)
    {
        i++;
        j = ft_strlen(temp->word);
        if (len < j)
            len = j;
        temp = temp->next;
    }
    arg = (char **)malloc(sizeof(char) * i * len);
    temp = token;
    j = 0;
    while (j < i)
    {
        arg[j] = ft_strdup(temp->word);
        temp = temp->next;
        j++;
    }
	arg[j] = NULL;
    return (arg);
}

char	*find_path(t_env *env, char *bin)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, bin))
			return (tmp->value);
		tmp = tmp->next;
	}
	return ("");
}

char 	*check_path(char **tmp, char *arg)
{
	DIR 	*dir;
	struct dirent *bin;
	int 	i;

	i = 0;
	while (tmp[i])
	{
		dir = opendir(tmp[i]);
		while (dir)
		{
			bin = readdir(dir);
			if (bin == 0)
				break ;
			else if (ft_strcmp(bin->d_name, arg) == 0)
			{
				closedir(dir);
				return (tmp[i]);
			}
		}
		i++;
		closedir(dir);		
	}
	return ("");
}

char	*make_path(char *arg, t_env *env)
{
	char	*path;
	char	**tmp;
	char	*bin;

	bin = find_path(env, "PATH");
	tmp = ft_split(bin, ':');
	path = ft_strdup(check_path(tmp, arg));
	path = ft_strjoin(path, "/");
	path = ft_strjoin(path, arg);
	return (path);
}

int		exec_bin(t_store *token, t_env *env_value, char **env)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**arg;
	
	arg = NULL;
	arg = make_arg(token, arg);
	path = make_path(arg[0], env_value);
	pid = fork();
	signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
	if (pid == 0)
	{
		t_sh.fork_status = 1;
		if (execve(path, arg, env) < 0)
		{
			printf("%d", errno);
			if (errno == 2 || errno == 14)
				printf("%s: command not found\n", arg[0]);
			else
				printf("%s\n", strerror(errno));
			t_sh.fork_status = 0;
			exit(1);
		}
		t_sh.fork_status = 0;
		exit(0);
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
	free(arg);
	return 1;
}


