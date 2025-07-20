#include "pipex.h"

void child1(int pipefd[2], char **argv, char **envp)
{
// ouvre le fichier en une seule lecture, quitte si echec
// faire open d'un fichier existant retourne un fd >= 0
    int infile = open(argv[1], O_RDONLY);
    if (infile < 0)
        error_exit("open infile");
// redirige le fichier infile vers std in du processus (prochain prog lancera depuis infile)
    dup2(infile, STDIN_FILENO);
// dup2 redirige la std out du processus vers l'écriture du pipe (écrit dans le pipe au lieu du terminal)
// tout ce qu'on printf après ca ira dans le pipe pas dans le terminal
    dup2(pipefd[1], STDOUT_FILENO);
// ferme tous les fd inutiles pour éviter fuites ou deadlocks
    close(pipefd[0]);
    close(pipefd[1]);
    close(infile);
// decoupe la cmd en tableau
    char **cmd_args = split_cmd(argv[2]);
// trouve le chemin absolu de la commande, si pas trouver exit
    char *cmd_path = find_cmd_path(cmd_args[0], envp);
    if (!cmd_path)
        error_exit("cmd1 not found");
// remplace le processus actuel par le programme trouve
// ex : remplacer ton process par /bin/ls, ou /usr/bin/grep etc..
    execve(cmd_path, cmd_args, envp);
// si execve echoue affiche l'erreur, libére la memoire et exit
    perror("execve");
    free_split(cmd_args);
    exit(EXIT_FAILURE);
}

void child2(int pipefd[2], char **argv, char **envp)
{
// ouvre le fichier outfile, le tronque s'il existe et permet a tout le monde de lire et a nous d'ecrire
    int outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (outfile < 0)
        error_exit("open outfile");
// redirige std in vers le pipe
    dup2(pipefd[0], STDIN_FILENO);
// redirige std out vers le fichier outfile
    dup2(outfile, STDOUT_FILENO);
    close(pipefd[1]);
    close(pipefd[0]);
    close(outfile);
// decoupe les arguments et chercher le path absolu
    char **cmd_args = split_cmd(argv[3]);
    char *cmd_path = find_cmd_path(cmd_args[0], envp);
    if (!cmd_path)
        error_exit("cmd2 not found");
// execute la cmd2 en lisant depuis le pipe et en ecrivant dans outfile
    execve(cmd_path, cmd_args, envp);
    perror("execve");
    free_split(cmd_args);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
    if (argc != 5) {
        fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }
// tableau pour deux fd (input, output)
// pipe utile pour échange de data entre processus sans passer par fichier ou réseaux
// processus pere écrit dans le pipe, le processus fils lit dedans, ou inversement
// pipefd[0] = lecture du contenu du pipe, pipefd[1] = écrire dans le pipe
    int pipefd[2];
    if (pipe(pipefd) == -1)
        error_exit("pipe");
// fonction fork : cree un nouveau processus fils casi identique a son processus pere
// pid_t : type utilise pour stocker les ID de processus
// pid1 : variable qui va contenir le résultat de fork
// on clone le processus en un child, ici on en fait 2
// si pid1 et pid2 == 0 on est dans le processus child, <0 erreur, et >0 dans le pere
    pid_t pid1 = fork();
    if (pid1 < 0)
        error_exit("fork");

    if (pid1 == 0)
        child1(pipefd, argv, envp);

    pid_t pid2 = fork();
    if (pid2 < 0)
        error_exit("fork");

    if (pid2 == 0)
        child2(pipefd, argv, envp);
// le processus pere n'a plus besoin du pipe, il le ferme
    close(pipefd[0]);
    close(pipefd[1]);
// le processus pere attend que les deux child terminent proprement
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}

