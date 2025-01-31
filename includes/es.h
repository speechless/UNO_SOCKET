#include <stdio.h>
#include <requetes.h>

//typedef int T_Elt;

//typedef int T_Elt[2];
typedef client_t T_Elt;


//typedef char T_chaine[26];
//typedef T_chaine T_Elt; //TAD
//typedef char  T_Elt; //TAD

//typedef char T_Elt[26];




void saisirElt(T_Elt*);
void afficherElt(T_Elt* e, char* end);
void affecterElt(T_Elt* dest, T_Elt* src);
int comparerElt(T_Elt* elt1, T_Elt* elt2);
