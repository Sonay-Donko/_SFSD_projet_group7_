#define _CRT_SECURE_NO_WARNINGS


#include "Header.h"
//#pragma once


TnOVnC* open(char* fileName, char mode) { // open , creat entet !


    TnOVnC* tnOVnC  = (TnOVnC*)malloc(sizeof(TnOVnC));
    if (tnOVnC == NULL) {
        perror("ERROR,   ouverture "); return NULL;
    }

    //char s[4];
    if ((mode == 'A') || (mode == 'a'))  (tnOVnC->mainFile) = fopen(fileName, "ab+");     //ouverture d'un nouveau fichier 
    else fopen(fileName, "wb+");        // creation de fichier
   
    if ((mode == 'A') || (mode == 'a'))                   //ouverture d'un ancien fichier
    {

        fread(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);

    }
    else
    {
        // Initialisation de l'entete
        tnOVnC->header.adrLastBloc = 0;      //d'adresse de dernier bloc=0
        tnOVnC->header.nbrEnreg = 0;            //le nombre des enregestrements=0
        tnOVnC->header.nbrSuprLogiq = 0;              //le nombre des enregestrements supprimés =0
        fwrite(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);
    }
    return tnOVnC;
}



//pour la fermeture de fichier  , libre lespace de pointer est ecrire la donne de la tete , est ferme le fichier 
int close(TnOVnC* tnOVnC) {

    rewind(tnOVnC->mainFile);               // mettre le curseur au debut 
   
        fwrite(&(tnOVnC->header), sizeof(Header), 1, tnOVnC->mainFile);  // Mise a jour de l'entête dans le fichier !    
        fclose(tnOVnC->mainFile);
    
        free(tnOVnC);   // libre la memoire 
        tnOVnC = NULL;      // juste pour assure 
 
        return 0;
}



int GetHeader(TnOVnC* tnOVnC, int i)   //fonction qui retourne les paramètres de l'entete
{
    if (i == 1) return ((tnOVnC->header).adrLastBloc); //si 1:retourner d'adresse de dernier bloc
    else if (i == 2) return ((tnOVnC->header).nbrEnreg);       //si 2:retourner nombre des enregestrements
    else ((tnOVnC->header).nbrSuprLogiq);                   //si 2:retourner nombre des enregestrements supprimés
}

void SetHeader(TnOVnC* tnOVnC, int i, int val)      //procédure pour la modification des paramètres de l'entete

{
    if (i == 1) (tnOVnC->header).adrLastBloc = val;  //modifier  d'adresse de dernier bloc
    else if (i == 2) (tnOVnC->header).nbrEnreg = val;   //modifier nombre des enregestrements
    else (tnOVnC->header).nbrSuprLogiq = val;               //modifier nombre des enregestrements supprimés
}


int liredir(TnOVnC* tnOVnC, int i, TBloc* buf)       //procédure pour la lecture d'un bloc
{

    if (i <= GetHeader(tnOVnC, 1))                        //si le numéro de bloc < netete(pf,1)
    {
      //  CptAccLect++;                           //on incrémante  CptAccLect
        fseek(tnOVnC->mainFile, sizeof(Header) + (i - 1) * sizeof(TBloc), SEEK_SET);
        fread(buf, sizeof(TBloc), 1, tnOVnC->mainFile);
        return 0;
    }
    else
    {
        printf("ERROR, liredir() le bloc nexiste pas !"); return -1;
    }
}

int ecriredir(TnOVnC* tnOVnC, int i, TBloc buf)     //procédure pour l'écriture d'un bloc
{

    if (i <= GetHeader(tnOVnC, 1))                       //si le numéro de bloc < netete(pf,1)
    {
    //    CptAccEcri++;                         // //on incrémante CptAccEcri
        fseek(tnOVnC->mainFile, sizeof(Header) + (i - 1) * sizeof(TBloc), SEEK_SET);
        fwrite(&buf, sizeof(TBloc), 1, tnOVnC->mainFile);
        return 0;
    }
    else
    {
        printf("ERROR, ecriredir() le bloc nexiste pas !"); return -1;
    }
}


int lireEtudiant(TBloc tBloc,int debutENreg, Etudiant* etudiant) {

    Enreg enreg;        // il a data on fomre de tab
  //  Etudiant etudiant;
   // fread(&tBloc, sizeof(TBloc), 1, tnOVnC->mainFile);

    for (int i =0; i< 3; i++) {

        enreg.size[i] = tBloc.bloc[debutENreg+i];               // lire la taille 
    }
    enreg.size[3] = '\0';                   // mettre la fin de string 

    etudiant->taille = atoi(enreg.size);

    for (int i = 0; i < 6; i++) {

        enreg.mattricule[i] = tBloc.bloc[debutENreg + i+3];
    }
    enreg.mattricule[6] = '\0';

    etudiant->matricule = atoi(enreg.mattricule);

    etudiant->efface = (tBloc.bloc[9] == '1') ? true : false;


    for (int i = 0; i < etudiant->taille; i++)
    {
        etudiant->etudiantData[i] = tBloc.bloc[debutENreg + i + 10];
    }
    etudiant->etudiantData[etudiant->taille] = '\0';

    return 0;
}

int ecrireEtudiant(TnOVnC* tnOVnC,TBloc* tBloc, int debutEnregistrement, Etudiant etudiant,int numBloc) {

    fseek(tnOVnC->mainFile, sizeof(Header) + (numBloc - 1) * sizeof(TBloc), SEEK_SET);

    sprintf(tBloc->bloc + debutEnregistrement, "%s%03d", tBloc->bloc, etudiant.taille);

    sprintf(tBloc->bloc + debutEnregistrement + 3, "%s%06d", tBloc->bloc, etudiant.matricule);

    (etudiant.efface == true) ? sprintf(tBloc->bloc + debutEnregistrement + 9, "%s%d", tBloc->bloc, 1) : sprintf(tBloc->bloc + debutEnregistrement + 9, "%s%d", tBloc->bloc, 0);

    sprintf(tBloc->bloc + debutEnregistrement + 10, "%s%s", tBloc, etudiant.etudiantData);

    fwrite(&tBloc, sizeof(TBloc), 1, tnOVnC->mainFile);
    return 0;
}

int rechercheEtudiant(TnOVnC* tnOVnC, int matricule,bool* trouv,Etudiant* etudiant) {
    //Buffer buf;
    //Enreg buf2;
    int blocPos=0;                                  // la position dans le bloc pour qu'il nous aide a determine la postion de etudinat dans le bloc 
    *trouv = false;
    //Etudiant etudiant;                          // ou on va lire lenreges
    TBloc* bufBloc = (TBloc*)malloc(sizeof(TBloc));
    for (int i = 1; i <= tnOVnC->header.adrLastBloc; i++) {

        liredir(tnOVnC, i,bufBloc);

        while (blocPos < TAILLE_DE_BLOC) {

           lireEtudiant(*bufBloc, blocPos,etudiant);
          if (etudiant->matricule == matricule) {

              printf("we found the student ");

              *trouv = true;

              return 0;
          }
          else {

              blocPos = etudiant->taille;

          }
        }


    }

    printf("we didnt found the student");

    return 0;
}