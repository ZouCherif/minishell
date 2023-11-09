/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des commandes internes du minishell (implémentation).
 
 */

#include "cmd.h"

int is_builtin(const char* cmd) {

}

int builtin(cmd_t* cmd) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argv[1] == NULL) {
            dprintf(cmd->stderr, "cd: argument manquant\n");
            return 1;
        }

        return cd(cmd->argv[1], cmd->stderr);
    }
    
    // Gérez d'autres commandes internes ici...
    
    return -1;  // Si la commande n'est pas reconnue.
}


int cd(const char* path, int fderr) {
   if (path == NULL) {
        dprintf(fderr, "cd: argument manquant\n");
        return 1;
    }

    if (chdir(path) != 0) {
        dprintf(fderr, "cd: impossible de changer de répertoire vers %s\n", path);
        return 1;
    }

    return 0;
}

int export(const char* var, const char* value, int fderr) {

}

int exit_shell(int ret, int fderr) {

}