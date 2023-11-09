/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des processus (implémentation).
 
 */

#include "cmd.h"

int exec_cmd(cmd_t* p) {

}

int init_cmd(cmd_t* p) {
    p->pid = -1;  // Valeur par défaut pour pid
    p->status = 0;  // Valeur par défaut pour status
    p->stdin = 0;  // Utilisation du descripteur de fichier standard pour stdin
    p->stdout = 1;  // Utilisation du descripteur de fichier standard pour stdout
    p->stderr = 2;  // Utilisation du descripteur de fichier standard pour stderr
    p->wait = 1;  // Valeur par défaut pour wait
    p->path = NULL;  // Valeur par défaut pour path
    for (int i = 0; i < MAX_CMD_SIZE; ++i) {
        p->argv[i] = NULL;  // Initialisation des pointeurs à NULL
        p->fdclose[i] = -1;  // Valeur par défaut pour les descripteurs à fermer
    }
    p->next = NULL;  // Valeur par défaut pour next
    p->next_success = NULL;  // Valeur par défaut pour next_success
    p->next_failure = NULL;  // Valeur par défaut pour next_failure
}

int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
  
}
