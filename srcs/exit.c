# include "minishell.h"


void free_struct_store(t_store *token)
{
    t_store *p;
    while(token != NULL)
        {
            p = token;
            token = token->next;
            if (p)
                free(p);
        }
}

void free_struct_env(t_env *env)
{
	t_env *temp;
    if (env)
	{
		while(env != NULL)
		{
			temp = env;
			free(temp->key);
			free(temp->value);
			env = env->next;
		}
	}
}


int		free_exit(t_store *store, t_env *env, int code)
{
	
	if (code == 1)
		ft_putstr_fd("Error\n", 2);
	free_struct_store(store);
	free_struct_env(env);
	exit(code);
}