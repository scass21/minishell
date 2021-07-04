/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkasandr <lkasandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 06:18:35 by scass             #+#    #+#             */
/*   Updated: 2021/07/04 17:35:03 by lkasandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int our_env(char **env)
// {
// 	int		i;
	
// 	i = 0;
// 	while (env[i] != NULL)
// 	{
// 		printf("%s\n", env[i]);
// 		i++;
// 	}
// 	return (0);
// }

int check_count(char **argv, int count)
{
	int i;

	i = 0;
	if (count > 1)
	{
		while (i < 2)
		{
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": ", 2);
			i++;
		}
		ft_putstr_fd("No such file or directory\n", 2);
		return (0);
	}
	return (1);
}

int our_env(t_env *env, char **argv, int count)
{
	t_env *p;
	int i;
	
	i = 0;
	if (!check_count(argv, count))
		return (0);
	p = env;
	while (p != NULL)
	{
		printf("%s=%s\n", p->key, p->value);
		p = p->next;
	}
	return (0);
}
