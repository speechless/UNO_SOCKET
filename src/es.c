/**
 * @file es.c
 */

#include "es.h"
#include <string.h>

 /**
  * Afficher un élément de type T_Elt
  * @param e Pointeur vers l'élément à afficher
  * @param end Chaîne de caractères à afficher après l'élément
  */
void afficherElt(T_Elt* e, char* end) {
	printf("{id = %d, fd = %d]%s", (*e).id, e->socket.fd, end);
}

/**
 * Affecter un élément de type T_Elt à un autre
 * @param dest Pointeur vers l'élément de destination
 * @param src Pointeur vers l'élément source
 */
void affecterElt(T_Elt* dest, T_Elt* src) {
	(*dest).id = (*src).id;
	(*dest).socket = (*src).socket;
	strcpy(dest->adresse, src->adresse);
	dest->port = src->port;
}

/**
 * Compare deux éléments
 * @param elt1 Premier élément
 * @param elt2 Deuxième élément
 * @return nombre négatif si elt1 < elt2, 0 si elt1 == elt2, nombre positif sinon
 */
int comparerElt(T_Elt* elt1, T_Elt* elt2) {
	return (*elt1).id - (*elt2).id;
}
