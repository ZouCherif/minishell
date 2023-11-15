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
  int max;
  
  while (1) {
    // Effacer les contenus de cmdline, cmdtoks et cmds
    //"memset" Cette fonction permet de remplir une zone mémoire, identifiée par son adresse et sa taille, avec une valeur précise. 
    memset(cmdline, 0, sizeof(cmdline));
    memset(cmdtoks, 0, sizeof(cmdtoks));
    memset(cmds, 0, sizeof(cmds));
    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    for (int i = 0; i < MAX_CMD_SIZE; i++) {
      init_cmd(&cmds[i]);    
    } 
    // Afficher un prompt
    printf("$ ");
    
    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) break;
    cmdline[strlen(cmdline)-1]='\0';
    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    trim(cmdline);
    //   - ajouter d'éventuels espaces autour de ; ! || && & ...

    // ajouter_espace_adroite(cmdline, 3, strlen(cmdline)+1);
    // ajouter_espace_agauche(cmdline, 3, strlen(cmdline)+1);
    // printf("%s\n", cmdline);





    // max = 0;
    // for (int i = 0; i < strlen(cmdline); i++) {
    //   if (strchr("><&|;",cmdline[i])){
    //     max = max + 2;
    //   }
    //   max++;
    // }
    separate_s(cmdline, MAX_LINE_SIZE);
    //clean(cmdline);
    //join_sep(cmdline, "><&|;2");

    //   - supprimer les doublons d'espaces

    // //   - traiter les variables d'environnement
    // substenv(cmdline, MAX_LINE_SIZE);

    // // Découper la ligne dans cmdtoks
    // strcut(cmdline, ' ', cmdtoks, MAX_CMD_SIZE);

    // // Traduire la ligne en structures cmd_t dans cmds
    // parse_cmd(cmdtoks, cmds, MAX_CMD_SIZE);

    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue

    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
    // for (current=cmds; current!=NULL; ) {
    //   // Lancer la commande
    //   printf("%d", exec_cmd(current));
    //   current=current->next;
    // }
  }
  
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
