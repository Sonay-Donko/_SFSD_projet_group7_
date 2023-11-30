//#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define TAILLE_DE_BLOC 1200

//     le bloc qu'on va utilise 
typedef struct {

	char bloc[TAILLE_DE_BLOC+1]; 

}TBloc;

typedef struct {
    bool efface;
	int taille;
	int matricule;
	char etudiantData[1190];

}Etudiant;

typedef Etudiant Buffer;

typedef struct Enreg
{
    char mattricule[6];
    char efface;
    char size[3];
    char info[1190]; /**chaine de caractère qui contiendra :
                    - 3bits == indiquant la taille de l'enregistrement
                    - 1 bit d'effacement
                    - 6bits = Clé (code)
                    - reste du code généré **/
} Enreg;

typedef struct Header
{
    int adrLastBloc;  //nombre de blocs utilisés
    int nbrEnreg;        //nombre d'enregistrement dans le fichiers (qui ne sont pas supprimés)
    int nbrSuprLogiq;          //nombre d'éléments supprimés logiquement
} Header;


typedef struct TnOVnC {

    FILE* mainFile;
    Header header;

}TnOVnC;

TnOVnC* open(char* fileName, char mode);  // open , creat entet !
int close(TnOVnC* tnOVnC);

void SetHeader(TnOVnC *tnOVnC,int i ,int val);     // 
int GetHeader(TnOVnC* tnOVnC,int i);   // val i pour choir quoi mettre a jour ! 

int liredir(TnOVnC* pF, int i, Etudiant* buf);  // lire un bloc 
int ecriredir(TnOVnC* tnOVnC, int i, TBloc buf);    // ecrire un bloc return -1 si lindex est depasse

int lireEtudiant(TBloc tBloc, int debutENreg, Etudiant* etudiant);  // lire un etudiant 
int ecrireEtudiant(TnOVnC* tnOVnC, TBloc* tBloc, int debutEnregistrement, Etudiant etudiant, int numBloc);

Etudiant rechercheEtudiant(TnOVnC* tnOVnC, int matricule, bool* trouv);