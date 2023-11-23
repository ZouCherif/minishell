/*
  Projet minishell - Licence 3 Info - PSI 2023
 
Nom : SAD SAOUD 
Prénom : Farid
Num. étudiant : 22312283
Groupe de projet :1-9
Date :24/11/2024
Nom : ZOUAOUI
Prénom : Mohamed Cherif
Num. étudiant : 22313975
Groupe de projet :1-9
Date :24/11/2024

 
  Gestion des processus (implémentation).
 
 */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "cmd.h"
#include "builtin.h"
#include "parser.h"

int exec_cmd(cmd_t* p) {
    //si le nom de la commande est NULL 
    if (p->path == NULL){
      return -1;  //commande echouée
    } 
    pid_t child_pid; // l'identifiant de processus fils
    if(is_builtin(p->path)==0){
        // Si c'est une commande builtin on l'exécute directement avec la fonction built-in
        return builtin(p);
    }
    //sinon on crée un  processus pour exécuter les commandes externes
    else if(child_pid = fork()){//processus père 
            
            if(p->stdin != 0) close(p->stdin);
            if(p->stdout != 1) close(p->stdout);
            // if(p->stderr != 2) close(p->stderr);
            if(p->wait){
                waitpid(child_pid, &p->status, 0);
            }

            //WEXITSTATUS sert à verifier si la cmd a touver un resultat ou affiche une erreur
            if (p->next_success != NULL){
                // si la commande avant && n'a pas trouve de resultat 
                if(WEXITSTATUS(p->status)){
                    // On n'aura pas besoin d'exécuter ce qui est aprés
                    p->next_success = NULL;
                }
            }else if (p->next_failure != NULL){
                //si la commande avant || est exécuté
                if(!WEXITSTATUS(p->status)){
                    //on n'aura besoin d'exécuter ce qui aprés
                    p->next_failure = NULL;
                }
            }

    }else{//processus fils
        p->pid=getpid();
        //dupliquer les descipteurs des fichiers standard de processus fils 
        //pour avoir ses propres decripteurs
        dup2(p->stdin,0);
        dup2(p->stdout,1);
        dup2(p->stderr,2);
        //on ferme tous les descipteurs de fichiers ouverts par la commande
        for (int *fd = p->fdclose; *fd != -1; ++fd) {
            close(*fd);
        }
        //exécuter la commande 
        int exec = execvp(p->path,p->argv);
        // si l'exécution n'est pas fait on retourne un message d'erreur
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

//une fonction qui permet d'ajouter un desripteur à la fin de tableau fd_close 
//si elle n'existe pas déja 
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
//fusionner deux tableaux de ligne de commandes
void merge_fdclose(int* tab1, int* tab2){
  for(int i = 0; i< MAX_CMD_SIZE; i++)
    add_fdclose(tab1, tab2[i]);
  for(int i = 0; i< MAX_CMD_SIZE; i++)
    add_fdclose(tab2, tab1[i]);
}

int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
    int idx_proc=0; // l'indice dans le tableau cmds
    int idx_arg=0;// le nombre des arguments
    int max_tok=0;// //le nombre de tokens dans le tableau
    while (tokens[max_tok] != 0){
      max_tok++;
    }
    for(int idx_tok = 0; idx_tok < max_tok; ++idx_tok) {//idx_tok : indice dans le tabkeau des tokens
    //comparer chaque token avec un mot clé et faire le traitement nécessaire
        if (strcmp(";", tokens[idx_tok])==0){
            cmds[idx_proc].next=&cmds[idx_proc + 1];// commande terminé, passer à l'autre commande 
            ++idx_proc;// incrémenter l'indice dans le tableau de commande
            idx_arg=0;//réinitialiser le nombre d'rguments à 0
            continue;
            //redirection vers un fichier
        }else if (strcmp(">", tokens[idx_tok])==0) {
            //ouvrir un fichier ou le créer s'il n'existe pas
            int fdout = open(tokens[idx_tok+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            //retourner un message d'erreur si une erreur est survenue lors de l'ouverture de  fichier
            if (fdout==-1) {
                perror("Erreur de redirection :");
                return -1;
            }
            //rediriger la sortie standard de la commande actuelle vers le fichier ouvert
            cmds[idx_proc].stdout=fdout;
            //on ajoute le descripteur de fichier vers la fin de tableau des descripteurs
            add_fdclose(cmds[idx_proc].fdclose, fdout);
            idx_tok++; // Pour prendre en compte le nom de fichier
            continue; // Token traité
         //redirection avec concatination de la sortie avec le contenu d'un fichier existant 
        }else if (strcmp(">>", tokens[idx_tok]) == 0) {
             //ouvrir un fichier
        int fdout = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
         //retourner un message d'erreur si une erreur est survenue lors de l'ouverture de  fichier
        if (fdout == -1) {
            perror("Erreur de redirection :");
            return -1;
        }
        //rediriger la sortie standard de la commande actuelle vers le fichier ouvert
        cmds[idx_proc].stdout = fdout;
        //on ajoute le descripteur de fichier vers la fin de tableau des descripteurs
        add_fdclose(cmds[idx_proc].fdclose, fdout);
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
        //redirection de la sortie d'erreur
        }else if (strcmp("2>", tokens[idx_tok]) == 0) {
            //ouvrir un fichier ou le créer s'il n'existe pas
        int fderr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
         //retourner un message d'erreur si une erreur est survenue lors de l'ouverture de  fichier
        if (fderr == -1) {
            perror("Erreur de redirection :");
            return -1;
        }
        //rediriger la sortie d'erreur de la commande actuelle vers le fichier ouvert
        cmds[idx_proc].stderr = fderr;
        //on ajoute le descripteur de fichier vers la fin de tableau des descripteurs
        add_fdclose(cmds[idx_proc].fdclose, fderr);
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
        }else if (strcmp("2>>", tokens[idx_tok]) == 0) {
             //ouvrir un fichier 
        int fderr = open(tokens[idx_tok + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        //retourner un message d'erreur si une erreur est survenue lors de l'ouverture de  fichier
        if (fderr == -1) {
            perror("Erreur de redirection :");
            return -1;
        }
        //rediriger la sortie d'erreur de la commande actuelle vers le fichier ouvert
        cmds[idx_proc].stderr = fderr;
        //on ajoute le descripteur de fichier vers la fin de tableau des descripteurs
        add_fdclose(cmds[idx_proc].fdclose, fderr);
        idx_tok++; // Pour prendre en compte le nom de fichier
        continue; // Token traité
        //redirection de la sortie d'erreur vers la sortie standard
        }else if (strcmp("2>&1", tokens[idx_tok]) == 0) {
        //mettre à jour les descripteurs
        //rediriger la sortie d'erreur vers  la sortie standard
        cmds[idx_proc].stderr = cmds[idx_proc].stdout;
        continue; // Token traité
        }
        //redirection de la sortie standard vers la sortie d'errur
        else if (strcmp(">&2", tokens[idx_tok]) == 0) {
            //mettre à jour les descripteurs
        //rediriger la sortie standard vers  la sortie d'erreur
            cmds[idx_proc].stdout = cmds[idx_proc].stderr;
            continue; // Token traité
        }
        //redirection de l'entrée standard à partie d'un fichier
        else if (strcmp("<", tokens[idx_tok]) == 0) {
            //ouvrir un fichier en mode lecteur
            int fdin = open(tokens[idx_tok + 1], O_RDONLY, 0644);
            //retourner un message d'erreur si une erreur est survenue lord d'ouverture de fichier
            if (fdin == -1) {
                perror("Erreur de redirection :");
                return -1;
            }
            //rediriger l'entrée standard qui va etre lire à partir d'un fichier
            cmds[idx_proc].stdin = fdin;
            //on ajoute le descripteur de fichier vers la fin de tableau des descripteurs
            add_fdclose(cmds[idx_proc].fdclose, fdin);
            idx_tok++; // Pour prendre en compte le nom de fichier
            continue; // Token traité
        }
        //Exécution des commandes en arriere plan 
        else if (strcmp("&", tokens[idx_tok]) == 0) {
            cmds[idx_proc].wait = 0;//la shell n'attend pas la fin de processus fils
            cmds[idx_proc].next = &cmds[idx_proc + 1];// passer à la prochaine commande
            ++idx_proc;//avancer dans le tableau des commandes
            idx_arg = 0;//réinitialiser le nombre d'arguments
            continue; // Token traité
        }
        else if (strcmp("&&", tokens[idx_tok]) == 0) {
            //on met à jour next_success de la commande actuelle vers la commande suivante 
            cmds[idx_proc].next_success = &cmds[idx_proc + 1];
            // la première commande  sera exécutée si la commande suivante est exécutée
            ++idx_proc;//avancer dans le tableau des commandes 
            idx_arg = 0;
            continue; // Token traité
        }
        else if (strcmp("||", tokens[idx_tok]) == 0) {
            //on met à jour next_failure de la commande actuelle vers la commande suivante 
            cmds[idx_proc].next_failure = &cmds[idx_proc + 1];
            //l'un des deux commandes sera exécutée
            ++idx_proc;
            idx_arg = 0;
            continue; // Token traité
        //pipe
        }else if (strcmp("|", tokens[idx_tok]) == 0) {
            int tube[2];
            //tube[0] l'extrimité de lecteur dans le tube 
            //tube[1] l'extrimité d'écriture dans le tube
            //retourner un message d'erreur si une erreur est survenue lors de la création de pipe
            if (pipe(tube) == -1) { 

                write(cmds[idx_proc].stderr, "Erreur creation du pipe\n",
                    sizeof("Erreur creation du pipe\n"));
            }
            //rediriger la sortie standard de la 1er commande vers l'extrimité d'ecriture de tube
            cmds[idx_proc].stdout = tube[1];
               //rediriger la sortie standard de la 1er commande vers l'extrimité de lecteur de tube
            cmds[idx_proc + 1].stdin = tube[0]; 
            //ajouter les descripteurs ouverts vers le tableau des descripteurs
            add_fdclose(cmds[idx_proc].fdclose, cmds[idx_proc].stdout);
            add_fdclose(cmds[idx_proc].fdclose, cmds[idx_proc + 1].stdin);

            // Commande suivante
            cmds[idx_proc].next = &cmds[idx_proc + 1];
            ++idx_proc;
            idx_arg = 0;

            continue; // Token traité
        }
        //traiter les affectations 
        else if(strchr(tokens[idx_tok],'=') != NULL){
            char var[50];//chaine de caractère pour stocker le nom de la variable
            char val[50];//chaine de caractère pour stocker la valeur de la variable
            int k = 0;
            int i;
            for (i = 0; i < strlen(tokens[idx_tok]); ++i) {
                //les caractère avant '=' seront stockés dans var 
                if (tokens[idx_tok][i] == '=') {
                    break;
                }
                var[k] = tokens[idx_tok][i];
                k++;
            }
            i++;
            k  = 0;
            while (i < strlen(tokens[idx_tok])) {
                //les caractère aprés '=' seront stockés dans var 
                val[k] = tokens[idx_tok][i];
                i++;
                k++;
            }
            //ajouter un variable d'environnement et lui affecter la valeur
            setenv(var, val, 1);
        }else if (strcmp("for", tokens[idx_tok]) == 0){
                int start, end;
                //on verifie si le 3eme tokens et de la forme {int..int}, si vrai on recupere les indices
                if (sscanf(tokens[idx_tok+3], "{%d..%d}", &start, &end) == 2){
                    char cmd[MAX_LINE_SIZE];
                    char* cmd_pieces[MAX_CMD_SIZE];
                    cmd_t* current;
                    //ici on fait le meme traitement que le main evec la commande à i'interieure de la boucle
                    printf("for> ");
                    if (fgets(cmd, MAX_LINE_SIZE, stdin)==NULL) break;
                    cmd[strlen(cmd)-1]='\0';
                    trim(cmd);
                    separate_s(cmd, MAX_LINE_SIZE);
                    clean(cmd);
                    substenv(cmd, MAX_LINE_SIZE);
                    strcut(cmd, ' ', cmd_pieces, MAX_CMD_SIZE);
                    parse_cmd(cmd_pieces, cmds, MAX_CMD_SIZE);
                    //on execute la commande {end-start} fois
                    for (int i = start; i < end; i++) {
                        for (current = cmds; current != NULL;) {
                            exec_cmd(current);
                            if (current->next != NULL) {
                                // Exécution inconditionnelle
                                current = current->next;
                                continue;
                            }
                            if (current->next_success != NULL) {
                                current = current->next_success;
                                continue;
                            }
                            if (current->next_failure != NULL) {
                                current = current->next_failure;
                                continue;
                            }
                            break;
                            }
                    }
                    return 3;
                }else {

                }
        }
        else{
            //une nouvelle commande
            if (idx_arg==0) {
                cmds[idx_proc].path = tokens[idx_tok];
            }
            // c'est un arguments de la commande actuelle
            cmds[idx_proc].argv[idx_arg++] = tokens[idx_tok];
        }
    }
    //fusionner les tableaux des descripteurs des commandes 
    for(int i = 0; i<MAX_CMD_SIZE; i++){
    for(int j = 0; j<MAX_CMD_SIZE; j++)
      merge_fdclose(cmds[i].fdclose, cmds[j].fdclose);
    }

  return 0;
}
