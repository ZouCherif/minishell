/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Interface du minishell.
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "cmd.h"

int main(int argc, char* argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t* current;
  
  while (1) {
    // Effacer les contenus de cmdline, cmdtoks et cmds
    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    
    // Afficher un prompt
    printf("$ ");
    
    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) break;
    cmdline[strlen(cmdline)-1]='\0';
    
    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    //   - ajouter d'éventuels espaces autour de ; ! || && & ...
    //   - supprimer les doublons d'espaces
    //   - traiter les variables d'environnement

    // Découper la ligne dans cmdtoks

    // Traduire la ligne en structures cmd_t dans cmds
    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue

    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
    for (current=cmds; current!=NULL; ) {
      
      // Lancer la commande
      //exec_cmd(current);
      
    }
  }
  
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
