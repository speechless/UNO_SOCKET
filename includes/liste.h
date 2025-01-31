#include "es.h"
#include <stdlib.h>

#define listeVide(L) (L==NULL)

typedef struct maille {
	T_Elt elt; // type T_Elt toujours déclaré dans es.h
	struct maille* suivant; //ici se trouve l’auto référence
	// struct maille *precedent; //à mettre en oeuvre plus tard
} T_Maille;



T_Maille* supprimerElement(T_Elt elt, T_Maille* liste);
int tailleListe(T_Maille* liste);


T_Maille* newMaille(T_Elt*, T_Maille*);
int appartient(T_Elt elt, T_Maille* liste);
int afficherListe(T_Maille* p);
T_Maille* insererEnTete(T_Elt* elt, T_Maille* liste);
T_Maille* insererEnFin(T_Elt* elt, T_Maille* liste, T_Maille** mailleInseree);
T_Maille* insererAvecOrdre(T_Elt elt, T_Maille* liste);
T_Elt* eltFromIndex(T_Maille* liste, int index);