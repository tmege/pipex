/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmege <tmege@student.42barcelona.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:31:48 by tmege             #+#    #+#             */
/*   Updated: 2025/07/22 17:35:01 by tmege            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env_path(char **envp)
{
	int		i;
	char	*current_env;

	i = 0;
	while (envp[i] != NULL)
	{
		current_env = envp[i];
		if (ft_strncmp(current_env, "PATH=", 5) == 0)
			return (current_env + 5);
		i++;
	}
	return (NULL);
}

char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*with_slash;
	char	*full_path;

	i = 0;
	while (paths[i] != NULL)
	{
		with_slash = ft_strjoin(paths[i], "/");
		if (with_slash == NULL)
			return (NULL);
		full_path = ft_strjoin(with_slash, cmd);
		free(with_slash);
		if (full_path == NULL)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = get_env_path(envp);
	if (path == NULL)
		return (NULL);
	paths = ft_split(path, ':');
	if (paths == NULL)
		return (NULL);
	full_path = search_in_paths(paths, cmd);
	free_split(paths);
	return (full_path);
}
