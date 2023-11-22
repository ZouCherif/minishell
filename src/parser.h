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
int separate_s(char* str, size_t max);
int join_sep(char* str, char* s);
int substenv(char* str, size_t max);

int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max);
#endif
