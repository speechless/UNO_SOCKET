/**
 * @file es.h
 */

#include <stdio.h>
#include <requetes.h>

 /**
  * Utilisation de la structure client_t pour les éléments
  */
typedef client_t T_Elt;

/**
 * Afficher un élément de type T_Elt
 * @param e Pointeur vers l'élément à afficher
 * @param end Chaîne de caractères à afficher après l'élément
 */
void afficherElt(T_Elt* e, char* end);

/**
 * Affecter un élément de type T_Elt à un autre
 * @param dest Pointeur vers l'élément de destination
 * @param src Pointeur vers l'élément source
 */
void affecterElt(T_Elt* dest, T_Elt* src);

/**
 * Compare deux éléments
 * @param elt1 Premier élément
 * @param elt2 Deuxième élément
 * @return nombre négatif si elt1 < elt2, 0 si elt1 == elt2, nombre positif sinon
 */
int comparerElt(T_Elt* elt1, T_Elt* elt2);
