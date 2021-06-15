/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkasandr <lkasandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 13:58:12 by scass             #+#    #+#             */
/*   Updated: 2021/06/14 18:40:44 by lkasandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int		our_echo(char **argv)
// {
// 	int		i;
// 	int		n;

// 	i = 1;
// 	n = 0;
// 	if (!argv)
// 		printf("\n");
// 	if (argv[i] && ft_strcmp(argv[i], "-n") == 0)
// 		n = 1;
// 	while (argv[i])
// 	{
// 		printf("%s", argv[i]);
// 		if (argv[i + 1])
// 			printf(" ");
// 		i++;
// 	}
// 	if (!n)
// 		printf("\n");
// 	return (0);	
// }

int		our_echo(char *argument)
{
	if (!argument)
		printf("\n");
	printf("%s", argument);
	return (0);	
}