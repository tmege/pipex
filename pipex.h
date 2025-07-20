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

void    error_exit(const char *msg);
char    **split_cmd(const char *cmd);
char    *find_cmd_path(char *cmd, char **envp);
void    free_split(char **strs);

#endif

