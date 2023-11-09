/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Parsing de la ligne de commandes utilisateur.
 
 */

#ifndef _PARSER_H
#define _PARSER_H

#define MAX_LINE_SIZE   1024
#define MAX_CMD_SIZE    256

/*
  Voir le TP sur les chaînes de caractères.
  Commentaires à compléter.
*/

int trim(char* str);
int clean(char* str);
int separate_s(char* str, char* s, size_t max);
int substenv(char* str, size_t max);

int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max);
#endif
