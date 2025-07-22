/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmege <tmege@student.42barcelona.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:00:34 by tmege             #+#    #+#             */
/*   Updated: 2025/07/22 17:29:43 by tmege            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include "libft/includes/libft.h"

void	error_exit(const char *msg);
char	**split_cmd(const char *cmd);
void	free_split(char **strs);
char	*get_env_path(char **envp);
char	*search_in_paths(char **paths, char *cmd);
char	*find_cmd_path(char *cmd, char **envp);
void	child1(int pipefd[2], char **argv, char **envp);
void	child2(int pipefd[2], char **argv, char **envp);
void	setup_pipeline(char **argv, char **envp);

#endif
