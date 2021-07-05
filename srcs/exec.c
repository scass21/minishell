# include "minishell.h"

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
		if (dir)
			closedir(dir);		
	}
	return ("");
}

char	*make_path_home(char *arg)
{
	DIR 	*dir;
	struct dirent *bin;
	dir = opendir(getenv("PWD"));
	while (dir)
	{
		bin = readdir(dir);
		if (bin == 0)
			break ;
		else if (ft_strcmp(bin->d_name, arg) == 0)
		{
			closedir(dir);
			return(getenv("PWD"));
		}
	}
	closedir(dir);
	return("");
}

char	*make_path(char *arg, t_env *env)
{
	char	*path;
	char	**tmp;
	char	*bin;

	if (ft_strncmp(arg, "./", 2) == 0)
	{
		arg = ft_substr(arg, 2, (ft_strlen(arg) - 2));
		path = ft_strdup(make_path_home(arg));
	}
	else
	{
		bin = find_path(env, "PATH");
		tmp = ft_split(bin, ':');
		path = ft_strdup(check_path(tmp, arg));
	}
	path = ft_strjoin(path, "/");
	path = ft_strjoin(path, arg);
	
	return (path);
}

int		exec_bin(char **arg, t_env *env_value, char **env)
{
	pid_t	pid;
	int		status;
	char	*path;
	
	path = make_path(arg[0], env_value);
	pid = fork();
	signal(SIGINT, our_sig_proc);
	signal(SIGQUIT, our_sig_proc);
	if (pid == 0)
	{
		t_sh.fork_status = 1;
		if (execve(path, arg, env) < 0)
		{
			if (errno == 2 || errno == 14)
				print_error(arg[0]);
			else
				print_error(strerror(errno));
			t_sh.fork_status = 0;
			exit(1);
		}
		t_sh.fork_status = 0;
		exit(0);
	}
	else if (pid < 0)
	{
		print_error(strerror(errno));
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


