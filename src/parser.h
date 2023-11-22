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
//Cette fonction permet de supprimer les espaces de début et de fin d'une chaine de caractères
int trim(char* str);

//Cette fonction permet de supprimer les espaces doubles dans une chaine de caractères
int clean(char* str);

//Cette fonction permet de mettre un espace avant et aprés d'un mot clé dans une chaine de caractères
int separate_s(char* str, size_t max);

//
int join_sep(char* str, char* s);

//Cette fonction permet de remplacer la variables d'environnement par leurs valeurs
int substenv(char* str, size_t max);

/*Cette fonction permet de découper un chaine de caractère en des mots 
qui sont separés par des espaces et les stocker
dans un tableau de chaine de caractères*/

int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max);
#endif
