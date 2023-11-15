/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des processus (implémentation).
 
 */
#include <fcntl.h>
#include <string.h>
#include "cmd.h"
#include "builtin.h"

int exec_cmd(cmd_t* p) {
    return builtin(p);
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
    int idx_proc=0;
    int idx_arg=0;
    int max_tok=0;
    while (tokens[max_tok] != 0){
      max_tok++;
    }
  for(int idx_tok = 0; idx_tok < max_tok; ++idx_tok) {
    if (strcmp(";", tokens[idx_tok])==0){
         cmds[idx_proc].next=&cmds[idx_proc];
        ++idx_proc;
        idx_arg=0;
        continue;
    }else if (strcmp(">", tokens[idx_tok])==0) {
        int fdout = open(tokens[idx_tok+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fdout==-1) {
            perror("Erreur de redirection :");
            return -1;
        }
        cmds[idx_proc].stdout=fdout;
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
    }else{
        if (idx_arg==0) {
            cmds[idx_proc].path = tokens[idx_tok];
        }
        cmds[idx_proc].argv[idx_arg++] = tokens[idx_tok];
    }
  }

  return 0;
}
