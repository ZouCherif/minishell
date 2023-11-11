/*
  Projet minishell - Licence 3 Info - PSI 2023
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

int trim(char* str) {
     if (str == NULL) {
        return -1; // Vérification pour éviter les pointeurs nuls
    }
    int length = strlen(str);
    int start = 0; // Indice du premier caractère non-espace
    int end = length - 1; // Indice du dernier caractère non-espace
    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }
    if (start == length) {
        str[0] = '\0';
        return 0;
    }
    while (str[end] == ' ' || str[end] == '\t') {
        end--;
    }
    int newLength = end - start + 1;
    memmove(str, str + start, newLength);
    str[newLength] = '\0';

    return 0;
}

int clean(char* str) {
  if (str == NULL) {
        return -1; // Vérification pour éviter les pointeurs nuls
    }

    int length = strlen(str);
    int i, j;

    for (i = 0, j = 0; i < length; i++) {
        if (str[i] == ' ' || str[i] == '\t') {
            // Si l'espace actuel est suivi d'un autre espace, sautez-le
            while (i < length - 1 && (str[i + 1] == ' ' || str[i + 1] == '\t')) {
                i++;
            }
        }
        str[j++] = str[i];
    }

    str[j] = '\0'; // Terminer la chaîne correctement

    return 0;
}

int separate_s(char* str, char* s, size_t max) {
int i=0,j=0;
    char tmp[max];
    while (i<max)
    {
        if (strchr(s,str[i])){
            tmp[j]=' ';
            j++;
            tmp[j]=str[i];
            j++;
            tmp[j]=' ';
            j++;
        }
        else {
            tmp[j]=str[i];
            j++;
        }
        i++;
    }
    //tmp[j]='\0';
    strcpy(str,tmp);
    trim(str);
    return 0;
}

int substenv(char* str, size_t max) {
    int i = 0, j = 0;
    char env[256];  // Augmentez la taille si nécessaire pour stocker des noms d'environnement plus longs
    char tmp[max];
    int k = 0;
    while (str[i] != '\0' && j < max) {
        if (str[i] == '$') {
            i++; // Avance pour vérifier le caractère suivant

            if (str[i] == '{') {
                i++; // Si le nom de l'environnement est entre {}, avancez pour le nom
                while (str[i] != '}' && str[i] != '\0' && k < 255) {
                    env[k] = str[i];
                    k++;
                    i++;
                }
                if (str[i] == '}') {
                    env[k] = '\0';
                } else {
                    env[0] = '\0'; // Le nom n'est pas correctement formaté, donc nous le mettons à une chaîne vide.
                }
            } else {
                while (str[i] != ' ' && str[i] != '\0' && k < 255) {
                    env[k] = str[i];
                    k++;
                    i++;
                }
                env[k] = '\0';
            }

            char* resenv = getenv(env);

            if (resenv != NULL) {
                strcpy(tmp + j, resenv);
                j += strlen(resenv);
            } else {
                strcpy(tmp + j, env);
                j += strlen(env);
            }

            k = 0;
        } else {
            tmp[j] = str[i];
            j++;
            i++;
        }
    }

    tmp[j] = '\0';

    if (j < max) {
        strcpy(str, tmp);
        return 0;
    } else {
        // La taille maximale a été dépassée
        return -1;
    }

}

int strcut(char* str, char sep, char** tokens, size_t max) {
     int i=0;
    char* token=strtok(str,&sep);
    while (i<max && token!=NULL)
    {
        tokens[i]=token;
        token=strtok(NULL,&sep);
        //printf("token : %s\n",tokens[i]);
        i++;
    }
    return 0;
}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {

}