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
#include "string.h"
#include "stdlib.h"
int is_builtin(const char* cmd) {
    // si cmd est l'une de ces commandes alors on retourne 0 pour dire vrai
    if (!strcmp(cmd,"cd")|| !strcmp(cmd,"export")|| !strcmp(cmd,"exit_shell"))
    {
        return 0;
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
            dprintf(cmd->stderr, "cd: Ajouter un argument !\n");
            return 1;
        }
        // si tout est bien on fait appel a la fonction cd
        return (cd(cmd->argv[1], cmd->stderr));
    }
    if (strcmp(cmd->argv[0], "export") == 0) {
        //argv[1] contient l'argument de la commande
        //On verifie si son contenu n'est pas vide
        if (cmd->argv[1] == NULL && cmd->argv[2]==NULL) {
            dprintf(cmd->stderr, "export contient deux arguments\n");
            return 1;
        }
        // si tout est bien on fait appel a la fonction cd
        return (export(cmd->argv[1],cmd->argv[2],cmd->stderr));
    }
    if (strcmp(cmd->argv[0], "exit") == 0) {
        return (exit_shell(cmd->argv[1], cmd->stderr));
    }
    return -1;  // Si la commande n'est pas reconnue.
}

int cd(const char* path, int fderr) {
   if (path == NULL) { // verifier que l'utilisateur a taper un argument avec cd
        dprintf(fderr, "cd: Ajouter un argument\n");
        return 1;
    }
// si le changement de repertoire n'est pas effectue on affiche un message d'erreur
    if (chdir(path) != 0) {
        dprintf(fderr, "cd: impossible de changer de répertoire vers %s\n", path);
        return 1;
    }
    return 0;
}

int export(const char* var, const char* value, int fderr) {
    // Verifier si les valeurs de "var" et "value" sont vides
    if (var == NULL || value == NULL) {
        dprintf(fderr, "Vous devez entrer la variable d'envirenement et sa valeur\n");
        return 1; // Error
    }
    printf("hello farid\n");
    // Affecter la valeur "value" to the variable envirenement
    if (setenv(var, value, 1) != 0) {
        dprintf(fderr, "Erreur dans la saisie de la variable envirenement\n");
        return 1; // Erreur
    }
    return 0; // succes
}

int exit_shell(int ret, int fderr) {
    if(ret==0){
        // 0 le code de succes pour la fonction exit()
        dprintf(fderr,"Sortie de minishell");
        exit(ret);
        //notre programme est termine avec succes
    }
    // sinon
    dprintf(fderr,"Une erreur s'est produite");
    exit(ret);
}