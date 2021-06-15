#include "minishell.h"

int	our_cd(int argc, char *path)			//argc - количество аргументов для cd
{
	if (argc == 1)
	{
		path = getenv("HOME");
		if(chdir(path) < 0)
			printf("%s\n", strerror(errno));
	}
	if (argc == 2)	
	{
		if (chdir(path) < 0)
			printf("%s\n", strerror(errno));
	}
	if (argc > 2)
		printf("cd: too many arguments\n");
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