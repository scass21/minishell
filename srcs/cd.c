#include "minishell.h"

static void change_env(t_env *env, char *path)
{
	t_env *p;
	t_env *p2;

	p = env;
	p2 = env;
	while(p)
	{
		if (ft_strcmp(p->key, "OLDPWD") == 0)
		{
			while(p2)
			{
				if (ft_strcmp(p2->key, "PWD") == 0)
				{
					free(p->value);
					p->value = ft_strdup(p2->value);
					break;
				}
				p2 = p2->next;
			}
		}
		if (ft_strcmp(p->key, "PWD") == 0)
		{
			free(p->value);
			p->value = ft_strdup(path);
		}
		p = p->next;
	}
}

static int check_argv(char **argv)
{
	int count;

	count = 0;
	while(argv[count])
		count++;
	if (count == 1 && ft_strlen(argv[0]) > 2)
		return (0);
	return (1);
}

int	our_cd(int argc, char **argv, t_env *env, t_env *export)			//argc - количество аргументов для cd
{
	char *r_path;
	char dir[4096];

	r_path = NULL;
	if (!check_argv(argv))
	{
		print_error(argv[0]);
		return (0);
	}
	// if (argc > 2)
	// {
	// 	printf("cd: too many arguments\n");
	// 	return (0);
	// }
	if (argc == 1)
	{
		argv[0] = getenv("HOME");
		if(chdir(argv[0]) < 0)
			print_error(strerror(errno));
	}
	if (argc >= 2)	
	{
		if (ft_strncmp(argv[1], "~", 1) == 0)
		{
			r_path = ft_strdup(getenv("HOME"));
			r_path = ft_strjoin(r_path, argv[1]);
			argv[1] = r_path;
			
		}
		if (chdir(argv[1]) < 0)
			print_error(strerror(errno));
	}
	argv[1] = getcwd(dir, 4096);
	change_env(env, argv[1]);
	change_env(export, argv[1]);
	return (0);
}

// int main(int argc, char **argv, char **env)
// {
// 	int		i;
// 	char	*path;

// 	if (argc == 1)
// 		path = getenv("HOME");
// 	else
// 		path = argv[1];
// 	char dir[4096];
// 	getcwd(dir, 4096);
// 	printf("%s\n", dir);
// 	// printf("%s\n", argv[1]);
	
// 	i = our_cd(argc, path);
// 	if (i < 0)
// 		printf("%s\n", strerror(errno));
// 	if (i == 0)
// 	{
// 		getcwd(dir, 4096);
// 		printf("%s\n", dir);
// 	}
// }