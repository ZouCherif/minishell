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
    //"memset" Cette fonction permet de remplir une zone mémoire, identifiée par son adresse et sa taille, avec une valeur précise. 
    memset(cmdline, 0, sizeof(cmdline));
    memset(cmdtoks, 0, sizeof(cmdtoks));
    memset(cmds, 0, sizeof(cmds));
    // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
    for (int i = 0; i < MAX_CMD_SIZE; i++) {
      init_cmd(&cmds[i]);    
    } 
    // Afficher un prompt
    printf("\033[1;32m\033[38;5;22m");
    printf("%s $ ", getenv("PWD"));
    printf("\033[0m");// Réinitialise la couleur à celle par défaut
    
    // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
    if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) break;
    cmdline[strlen(cmdline)-1]='\0';
    // Traiter la ligne de commande
    //   - supprimer les espaces en début et en fin de ligne
    trim(cmdline);
    //   - ajouter d'éventuels espaces autour de ; ! || && & ...

    separate_s(cmdline, MAX_LINE_SIZE);

    //   - supprimer les doublons d'espaces
    clean(cmdline);

    // //   - traiter les variables d'environnement
    substenv(cmdline, MAX_LINE_SIZE);

    // // Découper la ligne dans cmdtoks
    strcut(cmdline, ' ', cmdtoks, MAX_CMD_SIZE);

    // // Traduire la ligne en structures cmd_t dans cmds
    parse_cmd(cmdtoks, cmds, MAX_CMD_SIZE);


    // Les commandes sont chaînées en fonction des séparateurs
    //   - next -> exécution inconditionnelle
    //   - next_success -> exécution si la commande précédente réussit
    //   - next_failure -> exécution si la commande précédente échoue
    // Exécuter les commandes dans l'ordre en fonction des opérateurs
    // de flux
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
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}


//&, 2>&1