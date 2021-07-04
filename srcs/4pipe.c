# include "minishell.h"


int if_pipe(t_store *token, char **envp, t_env *export, t_env *env)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	t_store *tmp;

	tmp = token;

	if(pipe(fd) == -1)
		return (0);
	pid = fork();
	if (pid < 0)
	{
		printf("%s\n", strerror(errno));
		exit(1);
	}
	else if (pid == 0)
	{
		if (tmp->next)
			dup2(fd[1], 1);
		if (tmp->previous)
			dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execute_command(env, export, tmp, envp);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		t_sh.exit_code = WEXITSTATUS(status);
	
	
	return (0);
	
}