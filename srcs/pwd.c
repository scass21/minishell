/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scass <scass@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 00:29:04 by scass             #+#    #+#             */
/*   Updated: 2021/05/20 01:13:59 by scass            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	our_pwd(void)
{
	char dir[4096];
	
	errno = 0;
	if (getcwd(dir, 4096))
		printf("%s\n", dir);
	else
		printf("%s\n", strerror(errno));
	return 0;
}
