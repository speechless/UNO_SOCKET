#include "es.h"

/**
 * ======================================
 * QUAND T_ELT EST UN INT
 * ======================================
*/
/*
void saisirElt(T_Elt* e) {
	scanf("%d", e);
}
void afficherElt(T_Elt* e, char* end) {
	printf("%d%s", *e, end);
}
void affecterElt(T_Elt* d, T_Elt* s) {
	*d = *s;
}
int comparerElt(T_Elt* elt1, T_Elt* elt2) {
	return *elt1 - *elt2;
}
*/


/**
 * ======================================
 * QUAND T_ELT EST UN INT[2]
 * ======================================
*/
/*
void saisirElt(T_Elt* e) {
	scanf("%d %d", &(*e[0]), &(*e[1]));
}
void afficherElt(T_Elt* e, char* end) {
	printf("[%d, %d]%s", (*e)[0], (*e)[1], end);
}
void affecterElt(T_Elt* d, T_Elt* s) {
	(*d)[0] = (*s)[0];
	(*d)[1] = (*s)[1];
}
int comparerElt(T_Elt* elt1, T_Elt* elt2) {
	return (*elt1)[0] - (*elt2)[0];
}
*/


/**
 * ======================================
 * QUAND T_ELT EST UN client_t
 * ======================================
*/

void saisirElt(T_Elt* e) {
	printf("saisir NON IMPLEMENTE\n");
}
void afficherElt(T_Elt* e, char* end) {
	printf("{id = %d, fd = %d]%s", (*e).id, e->socket.fd, end);
}
void affecterElt(T_Elt* d, T_Elt* s) {
	(*d).id = (*s).id;
	(*d).socket = (*s).socket;
}
int comparerElt(T_Elt* elt1, T_Elt* elt2) {
	return (*elt1).id - (*elt2).id;
}
