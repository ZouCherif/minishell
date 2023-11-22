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

Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int trim(char* str) {
     if (str == NULL) {
        return -1; // Vérification pour éviter les pointeurs nuls
    }
    int length = strlen(str); // représente la longueur de la commande tapée
    int premierCar = 0; // représente l'indice de premier caractère aprés les espace 
    int dernierCar = length - 1; // représente l'indice de dernier caractère avant  les espace de fin
    // tant que le caractère est espace on incrémente l'indice premierCar
    while (str[premierCar] == ' ') {
        premierCar++;
    }
    //Si premierCar=length alors la commande tapée contient que des espaces
    if (premierCar == length) {
        str[0] = '\0'; //on met le caractère de fin de chaine dans str[0] la chaine contient que des espaces donc elle est vide
        return 0;
    }
    // tant que le caractère est espace on décrémente l'indice dernierCar
    while (str[dernierCar] == ' ') {
        dernierCar--;
    }
    //La nouvelle longueur sans espaces
    int newLength = dernierCar - premierCar + 1;
    //déplace la chaine str sans espaces de début et de fin vers le début de cette chaîne. 
    memmove(str, str + premierCar, newLength);
    //caractère de fin de chaine
    str[newLength] = '\0';

    return 0;
}

int clean(char* str) {

  if (str == NULL) {
        return -1; // Vérification pour éviter les pointeurs nuls
    }

    int length = strlen(str);//longueur de str
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
//cette fonction sera utilisé dans la fonction "separate_s" pour ajouter un espace aprés un mot clé
//position: la position de mot clé dans la chaine str
//len : est la longueur
void ajouter_espace_adroite(char* str, int position, int len) {
    char tmp[len];// une chaine temporaire
    int k = 0;
    for (int i = 0; i < strlen(str); i++) {
        tmp[k] = str[i];
        k++;
        if (i == position){
            //si la boucle arrive à la position de mot clé, on ajoute un espace aprés ce dernier et on continue la boucle 
            tmp[k] = ' ';
            k++;
        }
    }
    tmp[k] = '\0';//caractère de fin chaine
    //copier la chaine temporaire avec espace aprés les mots clé dans str 
    strcpy(str, tmp);
}
//cette fonction sera utilisé dans la fonction "separate_s" pour ajouter un espace avant un mot clé
//position: la position de mot clé dans la chaine str
//len : est la longueur
void ajouter_espace_agauche(char* str, int position, int len) {
    char tmp[len];
    int k = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (i == position){
            //si la boucle arrive à la position de mot clé, on ajoute un espace avant ce dernier et on continue la boucle
            tmp[k] = ' ';
            k++;
        }
        tmp[k] = str[i];
        k++;
    }
    tmp[k] = '\0';
    //copier la chaine temporaire avec espace avant les mots clé dans str 
    strcpy(str, tmp);
}

int separate_s(char* str, size_t max){
    int i = 0;
    while (str[i] != '\0') {
        //vérifier chaque caractère de la chaine str si c'est un mot clé
        //si oui on ajoute un espace avant et aprés ce mot clé
        switch (str[i]){
        case ';':
            ajouter_espace_agauche(str, i, strlen(str)+1);
            i++;
            ajouter_espace_adroite(str, i, strlen(str)+1);
            break;
        case '<':
            ajouter_espace_agauche(str, i, strlen(str)+1);
            i++;
            ajouter_espace_adroite(str, i, strlen(str)+1);
            break;
        case '&':
            ajouter_espace_agauche(str, i, strlen(str)+1);
            i++;
            if ((str[i+1] == '&') || (str[i+1]=='>')){
                i++;
            }
            ajouter_espace_adroite(str, i, strlen(str)+1);
            break;
        case '>':
            if (str[i+1] == '>'){
                ajouter_espace_agauche(str, i, strlen(str)+1);
                i += 2;
            }else if((str[i+1]=='&') && (str[i+2]=='2')){
                ajouter_espace_agauche(str, i, strlen(str)+1);
                i += 3;
            }else{
                ajouter_espace_agauche(str, i, strlen(str)+1);
                i++;
            }
            ajouter_espace_adroite(str, i, strlen(str)+1);
            break;
        case '|':
            ajouter_espace_agauche(str, i, strlen(str)+1);
            i++;
            if(str[i+1]=='|'){
                i++;
            }
            ajouter_espace_adroite(str, i, strlen(str)+1);
            break;
        case '2':
            if((str[i+1]=='>') && (str[i+2]=='&') && (str[i+3]=='1')){
                ajouter_espace_agauche(str, i, strlen(str)+1);
                i=i+4;
            }else if((str[i+1]=='>') && (str[i+2]=='>')){
                ajouter_espace_agauche(str, i, strlen(str)+1);
                i=i+3;
            }else if(str[i+1]=='>'){
                ajouter_espace_agauche(str, i, strlen(str)+1);
                i=i+2;
            }
            ajouter_espace_adroite(str, i, strlen(str)+1);
            break;
        }
        i++;
    }
    //effacer les espace de début et de fin
    trim(str);
}

int substenv(char* str, size_t max) {
    int i = 0, j = 0;
    char env[256];  // cette chaine est utilisée our stocker les noms des variables d'environnement
                    // Augmentez la taille si nécessaire pour stocker des noms d'environnement plus longs 
    char tmp[max];  // Chaine temporaire est utilisée pour stocker la nouvelle chaine avec les valeurs des variables d'env
                    // Avant les copier dans str 
    int k = 0;
    while (str[i] != '\0' && j < max) {
        if (str[i] == '$') {
            i++; // Avance pour vérifier le caractère suivant

            if (str[i] == '{') {
                i++; // Si le nom de l'environnement est entre {}, avancez pour le nom
                while (str[i] != '}' && str[i] != '\0' && k < strlen(env)) {
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
                while (str[i] != ' ' && str[i] != '\0' && k < strlen(env)) {
                    env[k] = str[i];
                    k++;
                    i++;
                }
                env[k] = '\0';
            }
            //chaine pour stocker la valeur de la variable d'environnement
            char* resenv = getenv(env);

            //si la valeur n'est pas null -- la variable d'env existe alors on copie la valeur dans tmp à partir de la postion j
            if (resenv != NULL) {
                strcpy(tmp + j, resenv);
                j += strlen(resenv);
            } else {
            //sinon on affiche rien
                strcpy(tmp + j, " ");
                j += strlen(" ");
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
    //on copie la chaine tmp dans str
        strcpy(str, tmp);
        return 0;
    } else {
        // La taille maximale a été dépassée
        return -1;
    }

}
//str :une chaine de caractère
//sep : un séparateur -- un caractère (c'est l'espace dans notre cas)
//tokens : un tableau de chaines de caratère 
//max : taille max
int strcut(char* str, char sep, char** tokens, size_t max) {
     int i=0;
     //extraire le premier token(mot) dans la chaine str en utilisant le séparateur sep 
    char* token=strtok(str,&sep);
    while (i<max && token!=NULL)
    {   
        //stocker chaque token extrait dans le tableau des tokens
        tokens[i]=token;
        token=strtok(NULL,&sep);
        i++;
    }
    return 0;
}

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max) {

}