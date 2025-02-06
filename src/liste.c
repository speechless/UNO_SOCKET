#include "liste.h"

/**
 * Crée une nouvelle maille
 * @param elt Le premier élément de la maille
 * @param suivant Le pointeur sur la maille suivante
 */
T_Maille* newMaille(T_Elt* elt, T_Maille* suivant) {
	T_Maille* maille = malloc(sizeof(T_Maille));
	affecterElt(&maille->elt, elt);
	maille->suivant = suivant;
	return maille;
}

/**
 * Insère un élément en tête de liste
 * @param elt Elément à insérer
 * @param liste Liste dans laquelle insérer
 * @return Un pointeur le le nouveau début de la liste
 */
T_Maille* insererEnTete(T_Elt* elt, T_Maille* liste) {
	if (listeVide(liste)) {
		T_Maille* maille = newMaille(elt, NULL);

		return maille;
	}

	T_Maille* maille = newMaille(elt, liste);
	return maille;
}

/**
 * Insère un élément en fin de liste
 * @param elt Elément à insérer
 * @param liste Liste dans laquelle insérer
 * @param mailleInseree Pointeur sur la maille insérée
 * @return Un pointeur sur le début de la liste
 */
T_Maille* insererEnFin(T_Elt* elt, T_Maille* liste, T_Maille** mailleInseree) {
	if (listeVide(liste)) {
		*mailleInseree = insererEnTete(elt, liste);
		return *mailleInseree;
	}

	T_Maille* mailleCourante = liste;

	while (mailleCourante->suivant != NULL) {
		mailleCourante = mailleCourante->suivant;
	}
	// mailleCourante correspond maintenant a la derniere de la liste
	mailleCourante->suivant = newMaille(elt, NULL);
	*mailleInseree = mailleCourante->suivant;
	return liste;
}

/**
 * Vérifie l'appartenence d'un élément dans une liste
 * @param elt Elément à chercher
 * @param liste Liste dans laquelle chercher
 * @return 1 si l'élément est dans la liste, 0 sinon
 */
int appartient(T_Elt elt, T_Maille* liste) {
	T_Maille* p = liste;

	while (!listeVide(p)) {
		if (comparerElt(&p->elt, &elt) == 0) return 1;
		p = p->suivant;
	}

	return 0;
}

/**
 * Affiche le contenu de la liste chaînée
 * @param maille Pointeur sur la liste à afficher
 * @return 0 si la liste est vide, 1 sinon
 */
int afficherListe(T_Maille* maille) {
	T_Maille* p = maille;

	if (listeVide(p)) return 0;

	while (!listeVide(p)) {
		afficherElt(&p->elt, " | ");
		printf("%p\n", p->suivant);
		p = p->suivant;
	}

	return 1;
}

/**
 * Supprime un élément de la liste
 * @param elt Elément à supprimer
 * @param liste Liste dans laquelle supprimer l'élément
 */
T_Maille* supprimerElement(T_Elt elt, T_Maille* liste) {
	if (!appartient(elt, liste)) {
		return liste;
	}

	if (comparerElt(&liste->elt, &elt) == 0) {
		return liste->suivant;
	}

	T_Maille* mailleCourante = liste->suivant;
	T_Maille* maillePrecedente = liste;

	while (mailleCourante != NULL) {
		if (comparerElt(&mailleCourante->elt, &elt) == 0) {
			maillePrecedente->suivant = mailleCourante->suivant;
			mailleCourante = mailleCourante->suivant;
		}
		else {
			maillePrecedente = mailleCourante;
			mailleCourante = mailleCourante->suivant;
		}
	}

	return liste;
}

/**
 * Retourne la taille de la liste
 * @param liste : la liste dont on veut connaître la taille
 * @return La taille de la liste
 */
int tailleListe(T_Maille* liste) {
	int taille = 0;

	T_Maille* p = liste;

	while (p != NULL) {
		taille++;
		p = p->suivant;
	}

	return taille;
}

/**
 * Cherche un élément de la liste à partir de son index
 * @param liste Liste dans laquelle chercher
 * @param index Index de l'élément à retourner
 * @return Un pointeur sur l'élément trouvé, NULL si non trouvé
 */
T_Elt* eltFromIndex(T_Maille* liste, int index) {
	//T_Maille* l = liste;

	if (listeVide(liste)) {
		return NULL;
	}

	int currentIndex = 0;
	while (currentIndex != index && !listeVide(liste)) {
		currentIndex++;
		liste = liste->suivant;
	}

	if (listeVide(liste)) {
		printf("index %d depasse la taille de la liste : ", index);
		return NULL;
	}
	else {
		return &liste->elt;
	}
}