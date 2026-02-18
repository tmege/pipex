/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmege <tmege@student.42barcelona.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:00:46 by tmege             #+#    #+#             */
/*   Updated: 2025/08/13 19:12:51 by tmege            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(const char *msg, char **to_free)
{
	if (msg)
		perror(msg);
	if (to_free)
		free_split(to_free);
	exit(EXIT_FAILURE);
}

char	**split_cmd(const char *cmd)
{
	return (ft_split(cmd, ' '));
}

void	free_split(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}
