/**
 * @file liste.h
 */

#include "es.h"
#include <stdlib.h>

 /**
  * Vérifie si une liste est vide
  */
#define listeVide(L) (L==NULL)

  /**
   * Structure de liste chainée
   */
typedef struct maille {
	T_Elt elt; /**< élément de la maille courante */
	struct maille* suivant; /**< pointeur sur l'élément suivant */
} T_Maille;


/**
 * Supprime un élément de la liste
 * @param elt Elément à supprimer
 * @param liste Liste dans laquelle supprimer l'élément
 */
T_Maille* supprimerElement(T_Elt elt, T_Maille* liste);

/**
 * Retourne la taille de la liste
 * @param liste : la liste dont on veut connaître la taille
 * @return La taille de la liste
 */
int tailleListe(T_Maille* liste);

/**
 * Crée une nouvelle maille
 * @param elt Le premier élément de la maille
 * @param suivant Le pointeur sur la maille suivante
 */
T_Maille* newMaille(T_Elt* elt, T_Maille* suivant);

/**
 * Vérifie l'appartenence d'un élément dans une liste
 * @param elt Elément à chercher
 * @param liste Liste dans laquelle chercher
 * @return 1 si l'élément est dans la liste, 0 sinon
 */
int appartient(T_Elt elt, T_Maille* liste);

/**
 * Affiche le contenu de la liste chaînée
 * @param maille Pointeur sur la liste à afficher
 * @return 0 si la liste est vide, 1 sinon
 */
int afficherListe(T_Maille* maille);

/**
 * Insère un élément en tête de liste
 * @param elt Elément à insérer
 * @param liste Liste dans laquelle insérer
 * @return Un pointeur le le nouveau début de la liste
 */
T_Maille* insererEnTete(T_Elt* elt, T_Maille* liste);

/**
 * Insère un élément en fin de liste
 * @param elt Elément à insérer
 * @param liste Liste dans laquelle insérer
 * @param mailleInseree Pointeur sur la maille insérée
 * @return Un pointeur sur le début de la liste
 */
T_Maille* insererEnFin(T_Elt* elt, T_Maille* liste, T_Maille** mailleInseree);

/**
 * Cherche un élément de la liste à partir de son index
 * @param liste Liste dans laquelle chercher
 * @param index Index de l'élément à retourner
 * @return Un pointeur sur l'élément trouvé, NULL si non trouvé
 */
T_Elt* eltFromIndex(T_Maille* liste, int index);