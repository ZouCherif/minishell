/*
  Projet minishell - Licence 3 Info - PSI 2023
  //Etudiant 1 : 
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
  Gestion des commandes internes du minishell (implémentation).

 */

#include "cmd.h"
#include "string.h"
#include "stdlib.h"
int is_builtin(const char* cmd) {
    // si cmd est l'une de ces commandes alors on retourne 0 pour dire vrai
    if (!strcmp(cmd,"cd")|| !strcmp(cmd,"export")|| !strcmp(cmd,"unset") || !strcmp(cmd,"exit"))
    {
        return 0; // succés ----> commande est built-in
    }
    //sinon on retourne 1 pour dire faux
    return 1;
}

int builtin(cmd_t* cmd) {
    //tester si le commande est "cd"
    if (strcmp(cmd->argv[0], "cd") == 0) {
        //argv[1] contient l'argument de la commande'
        //On verifie si son contenu n'est pas vide
        if (cmd->argv[1] == NULL) {
            cmd->argv[1] = "/home/cherif";
        }
        // si tout est bien on fait appel a la fonction cd
        return (cd(cmd->argv[1], cmd->stderr));
    }
    if (strcmp(cmd->argv[0], "export") == 0) {
        
        //On verifie si son contenu n'est pas vide
        if (cmd->argv[1] == NULL && cmd->argv[2]==NULL) {
            dprintf(cmd->stderr, "export contient deux arguments\n");
            return 1;
        }
        // si tout est bien on fait appel a la fonction cd
        return (export(cmd->argv[1], cmd->stderr));
    }
    if (strcmp(cmd->argv[0], "unset") == 0) {
        return (unset(cmd->argv[1], cmd->stderr));
    }
    if (strcmp(cmd->argv[0], "exit") == 0) {
        return (exit_shell(cmd->argv[1], cmd->stderr));
    }
    return -1;  // Si la commande n'est pas reconnue.
}

int cd(const char* path, int fderr) {
    char current_dir[1024];
    //On affecte le répertoire actuel dans la variable OLDPWD avant de le changer
    setenv("OLDPWD", getcwd(current_dir, sizeof(current_dir)), 1);
// si le changement de repertoire n'est pas effectue on affiche un message d'erreur
    if (chdir(path) != 0) {
        dprintf(fderr, "cd: impossible de changer de répertoire vers %s\n", path);
        return 1;
    }
    //On met à jour la valeur de PWD (répertoire courant)
    setenv("PWD", getcwd(current_dir, sizeof(current_dir)), 1);
    return 0;
}

int export(const char* var, int fderr) {
    // Verifier si les valeurs de "var" sont vides
    if (var == NULL) {
        dprintf(fderr, "Vous devez entrer la variable d'envirenement\n");
        return 1; // Error
    }
    // Affecter la valeur "value" to the variable envirenement
    if (setenv(var, getenv(var), 1) != 0) {
        dprintf(fderr, "Erreur dans la saisie de la variable envirenement\n");
        return 1; // Erreur
    }
    return 0; // succés
}

int unset(const char* var, int fderr) {
  //Si la variable d'environnement n'existe pas on retourne un message d'erreur 
  if(getenv(var)==NULL){
    write(fderr, "Variable n'existe pas\n", strlen("Variable n'existe pas\n"));
    return -1;
  }else{
    //sinon on supprime la variable d'environnement var
    unsetenv(var);
  }
  return 0; //succés
}

int exit_shell(int ret, int fderr) {
    if(ret==0){
        // 0 le code de succes pour la fonction exit()
        dprintf(fderr,"Sortie de minishell\n");
        exit(ret);
        //notre programme est termine avec succes
    }
    // sinon
    dprintf(fderr,"Une erreur s'est produite\n");
    exit(ret);
}