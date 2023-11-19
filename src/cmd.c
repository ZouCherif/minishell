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
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "cmd.h"
#include "builtin.h"

int exec_cmd(cmd_t* p) {
    if (p->path == NULL){
      return -1;  
    } 
    if(is_builtin(p->path)==0){
        return builtin(p);
    }
    else if(fork()){
            if(p->stdin != 0) close(p->stdin);
            if(p->stdout != 1) close(p->stdout);
            if(p->stderr != 2) close(p->stderr);
            if(p->wait){
                wait(&p->status);
            }

    }else{
        p->pid=getpid();
        dup2(p->stdin,0);
        dup2(p->stdout,1);
        dup2(p->stderr,2);
        for (int *fd = p->fdclose; *fd != -1; ++fd) {
            close(*fd);
        }
        int exec = execvp(p->path,p->argv);
        if(exec==-1){
            dprintf(p->stderr,"Commande inexistante\n");
            exit(-1);
        }
    }
    return 0;
}

int init_cmd(cmd_t* p) {
    p->pid = getpid();  // Valeur par défaut pour pid
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

void add_fdclose(int* fdclose, int fd){
    int i = 0;
    while (i < MAX_CMD_SIZE) {
        if(fdclose[i]==fd) break;
        if(fdclose[i]==-1){
            fdclose[i] = fd;
            break;
        }
        i++;
    }
}

void merge_fdclose(int* tab1, int* tab2){
  for(int i = 0; i< MAX_CMD_SIZE; i++)
    add_fdclose(tab1, tab2[i]);
  for(int i = 0; i< MAX_CMD_SIZE; i++)
    add_fdclose(tab2, tab1[i]);
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
            cmds[idx_proc].next=&cmds[idx_proc + 1];
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
            add_fdclose(cmds[idx_proc].fdclose, fdout);
            idx_tok++; // Pour prendre en compte le nom de fichier
            continue; // Token traité
        }else if (strcmp(">>", tokens[idx_tok]) == 0) {
        int fdout = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fdout == -1) {
            perror("Erreur de redirection :");
            return -1;
        }
        cmds[idx_proc].stdout = fdout;
        add_fdclose(cmds[idx_proc].fdclose, fdout);
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
        }else if (strcmp("2>", tokens[idx_tok]) == 0) {
        int fderr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fderr == -1) {
            perror("Erreur de redirection :");
            return -1;
        }
        cmds[idx_proc].stderr = fderr;
        add_fdclose(cmds[idx_proc].fdclose, fderr);
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
        }else if (strcmp("2>>", tokens[idx_tok]) == 0) {
        int fderr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fderr == -1) {
            perror("Erreur de redirection :");
            return -1;
        }
        cmds[idx_proc].stderr = fderr;
        add_fdclose(cmds[idx_proc].fdclose, fderr);
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
        }else if (strcmp("2>&1", tokens[idx_tok]) == 0) {
        cmds[idx_proc].stderr = cmds[idx_proc].stdout;
        continue; // Token traité
        }
        else if (strcmp(">&2", tokens[idx_tok]) == 0) {
            cmds[idx_proc].stdout = cmds[idx_proc].stderr;
            continue; // Token traité
        }
        else if (strcmp("<", tokens[idx_tok]) == 0) {
            int fdin = open(tokens[idx_tok + 1], O_RDONLY, 0644);
            if (fdin == -1) {
                perror("Erreur de redirection :");
                return -1;
            }
            cmds[idx_proc].stdin = fdin;
            add_fdclose(cmds[idx_proc].fdclose, fdin);
            idx_tok++; // Pour prendre en compte le nom de fichier
            continue; // Token traité
        }
        else if (strcmp("&", tokens[idx_tok]) == 0) {
            cmds[idx_proc].wait = 0;
            cmds[idx_proc].next = &cmds[idx_proc + 1];
            ++idx_proc;
            idx_arg = 0;
            continue; // Token traité
        }
        else if (strcmp("&&", tokens[idx_tok]) == 0) {
            cmds[idx_proc].next_success = &cmds[idx_proc + 1];
            ++idx_proc;
            idx_arg = 0;
            continue; // Token traité
        }
        else if (strcmp("||", tokens[idx_tok]) == 0) {
            cmds[idx_proc].next_failure = &cmds[idx_proc + 1];
            ++idx_proc;
            idx_arg = 0;
            continue; // Token traité
        }else if (strcmp("|", tokens[idx_tok]) == 0) {
            int tube[2];
            if (pipe(tube) == -1) { 
                write(cmds[idx_proc].stderr, "Erreur creation du pipe\n",
                    sizeof("Erreur creation du pipe\n"));
            }
            
            cmds[idx_proc].stdout = tube[1];
            cmds[idx_proc + 1].stdin = tube[0]; 
            
            add_fdclose(cmds[idx_proc].fdclose, cmds[idx_proc].stdout);
            add_fdclose(cmds[idx_proc].fdclose, cmds[idx_proc + 1].stdin);

            // Commande suivante
            cmds[idx_proc].next = &cmds[idx_proc + 1];
            ++idx_proc;
            idx_arg = 0;

            continue; // Token traité
        }
        else if(strchr(tokens[idx_tok],'=') != NULL){
            char var[50];
            char val[50];
            int k = 0;
            int i;
            for (i = 0; i < strlen(tokens[idx_tok]); ++i) {
                if (tokens[idx_tok][i] == '=') {
                    break;
                }
                var[k] = tokens[idx_tok][i];
                k++;
            }
            i++;
            k  = 0;
            while (i < strlen(tokens[idx_tok])) {
                val[k] = tokens[idx_tok][i];
                i++;
                k++;
            }
            setenv(var, val, 1);
        }
        else{
            if (idx_arg==0) {
                cmds[idx_proc].path = tokens[idx_tok];
            }
            cmds[idx_proc].argv[idx_arg++] = tokens[idx_tok];
        }
    }
    for(int i = 0; i<MAX_CMD_SIZE; i++){
    for(int j = 0; j<MAX_CMD_SIZE; j++)
      merge_fdclose(cmds[i].fdclose, cmds[j].fdclose);
    }

  return 0;
}
