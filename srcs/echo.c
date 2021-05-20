/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scass <scass@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 13:58:12 by scass             #+#    #+#             */
/*   Updated: 2021/05/19 14:03:27 by scass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		our_echo(char **argv)
{
	int		i;
	int		n;

	i = 1;
	n = 0;
	if (!argv)
		printf("\n");
	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
		n = 1;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!n)
		printf("\n");
	return (0);	
}