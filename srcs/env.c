/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkasandr <lkasandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 06:18:35 by scass             #+#    #+#             */
/*   Updated: 2021/06/21 00:48:57 by lkasandr         ###   ########.fr       */
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

int our_env(t_env *env)
{
	t_env *p;

	p = env;
	while (p != NULL)
	{
		printf("%s=%s\n", p->key, p->value);
		p = p->next;
	}
	return (0);
}
