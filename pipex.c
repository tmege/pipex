/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmege <tmege@student.42barcelona.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:59:41 by tmege             #+#    #+#             */
/*   Updated: 2025/08/13 19:19:14 by tmege            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child1(int pipefd[2], char **argv, char **envp)
{
	int		infile;
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = NULL;
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		error_exit("open infile", cmd_args);
	dup2(infile, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	cmd_args = split_cmd(argv[2]);
	if (!cmd_args)
		error_exit("split_cmd", cmd_args);
	cmd_path = find_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
		error_exit("cmd1 not found", cmd_args);
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	free_split(cmd_args);
	exit(EXIT_FAILURE);
}

void	child2(int pipefd[2], char **argv, char **envp)
{
	int		outfile;
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = NULL;
	outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
		error_exit("open outfile", cmd_args);
	dup2(pipefd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(pipefd[1]);
	close(pipefd[0]);
	close(outfile);
	cmd_args = split_cmd(argv[3]);
	if (!cmd_args)
		error_exit("split_cmd", cmd_args);
	cmd_path = find_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
		error_exit("cmd2 not found", cmd_args);
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	free_split(cmd_args);
	exit(EXIT_FAILURE);
}

void	setup_pipeline(char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		error_exit("pipe", NULL);
	pid1 = fork();
	if (pid1 < 0)
		error_exit("fork", NULL);
	if (pid1 == 0)
		child1(pipefd, argv, envp);
	pid2 = fork();
	if (pid2 < 0)
		error_exit("fork", NULL);
	if (pid2 == 0)
		child2(pipefd, argv, envp);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	const char	*msg;

	if (argc != 5)
	{
		msg = "Usage: ./pipex infile cmd1 cmd2 outfile\n";
		write(2, msg, ft_strlen(msg));
		exit(EXIT_FAILURE);
	}
	setup_pipeline(argv, envp);
	return (0);
}
