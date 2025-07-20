#include "pipex.h"

void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

char **split_cmd(const char *cmd)
{
    return ft_split(cmd, ' ');
}

// reproduit le comportement du shell pour trouver les commandes dans le PATH
char *find_cmd_path(char *cmd, char **envp)
{
// verifie si cmd existe et est executable, si oui, retourne copie via strdup
    if (access(cmd, X_OK) == 0)
        return ft_strdup(cmd);
// on boucle sur envp (tableau de variables d'environnement en cherchant la ligne
// qui commence par "PATH=", on récupère la partie après le '='
    char *path = NULL;
    for (int i = 0; envp[i]; i++) {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0) {
            path = envp[i] + 5;
            break;
        }
    }
// si pas de PATH on abandonne
    if (!path)
        return NULL;
// decoupe du PATH en morceaux
    char **paths = ft_split(path, ':');
    if (!paths)
        return NULL;
// boucle sur chaque chemin pour tester dir + "/" + cmd voir si executable
// on construit le chemin complet
    for (int i = 0; paths[i]; i++) {
        char *with_slash = ft_strjoin(paths[i], "/");
        char *full_path = ft_strjoin(with_slash, cmd);
        free(with_slash);
// Libere with slash acres avoir construit full path
// On verify si le chemin est executable et on retourne le chemin absolu
        if (access(full_path, X_OK) == 0) {
            free_split(paths);
            return full_path;
        }
        free(full_path);
    }
    free_split(paths);
    return NULL;
}

void free_split(char **strs)
{
    for (int i = 0; strs[i]; i++)
        free(strs[i]);
    free(strs);
}

