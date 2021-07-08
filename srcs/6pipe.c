# include "minishell.h"

int		token_size(t_store *token)
{
	t_store	*tmp;
	int			i;

	tmp = token;
	i = 0;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

int	check_redirect_in_pipe(char *word)
{
	if (ft_strchr(word, '>') || ft_strchr(word, '<'))
		return (1);
	else
		return (0);
}

char **get_pipe_array(t_store *token)
{
	t_store *p;
	char **array;
	char *str;

	p = token;
	str = NULL;
	while(p)
	{
		if (!str)
			str = ft_strdup(p->word);
		else
			str = ft_strjoin(str, p->word);
		p = p->next;
	}
	array = ft_split(str, '|');
	free(str);
	if (!array)
		ft_error(1);
	return (array);
}


t_store *get_pipe_struct(t_store *token)
{
	t_store *tmp;
	char **array;
	int k;

	k = 0;
	array = get_pipe_array(token);
	tmp = (t_store *)malloc(sizeof(t_store));
	tmp->word = NULL;
	tmp->next = NULL;
	while(array[k])
	{
		if (*array[k] == ' ')
			array[k]++;
		if (!tmp->word)
			tmp->word = ft_strdup(array[k]);
		else
			tmp = add_node_token(tmp, array[k], ft_strlen(array[k]));
		k++;
	}
	return (tmp);
}

int	pipe_proc(t_env *env, char **envp, t_env *export, t_store *token)
{
	int			fd[2];
	int			fd2[2];
	int			i;
	pid_t		pid[4000];
	t_store		*tmp;
	int			status;

	i = 0;
	tmp = get_pipe_struct(token);

	while (tmp)
	{
		if(pipe(fd) == -1)
			return (0);
		pid[i] = fork();
		if (pid < 0)
		{
			ft_putendl_fd(strerror(errno), 2);
			exit(1);
		}
		if (pid[i] == 0)
		{
			if (i == 0)			// первый пайп
			{	
				close(fd[0]);
				dup2(fd[1], 1);
			}
			else if (i == token_size(tmp))	// последний
			{
				close(fd[1]);
				dup2(fd2[0], 0);
			}
			else				// посередине
			{
				close(fd[0]);
				dup2(fd[1], 1);
				close(fd2[1]);
				dup2(fd2[0], 0);
			}
			if (check_redirect_in_pipe(tmp->word))
			{
				t_store *redir;
				char *word;
				int i;

				word = NULL;
				i = 0;
				redir = (t_store *)malloc(sizeof(t_store));
				if (!redir)
					ft_error(1);
				redir->word = NULL;
				redir->next = NULL;
				while(tmp->word[i] != '>' && tmp->word[i] != '<')
					i++;
				if (!redir->word)
					redir->word = ft_substr(tmp->word, 0, i);
				else
				{
					word = ft_substr(tmp->word, i, ft_strlen(tmp->word) - i);
					redir = add_node_token(redir, word, ft_strlen(word));
				}
				execute_command(env, export, redir, envp);
			}	
			else
				command_handler(env, export, tmp, envp);
			exit(0);
		}
		else
		{
			if (i > 0)
				close(fd2[0]);
			close(fd[1]);
			fd2[0] = fd[0];
			fd2[1] = fd[1];
			tmp = tmp->next;
			i++;
		}
	}
	while (i >= 0)			//ждём всех дочек
	{
		waitpid(pid[i], &status, 0);
			if (WIFEXITED(status))
				t_sh.exit_code = WEXITSTATUS(status);
		i--;
	}
	return (1);
}
