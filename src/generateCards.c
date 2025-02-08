#include "generateCards.h"


/**
 * Génère toutes les cartes possibles dans un tableau alloué dynamiquement.
 * @return Un pointeur vers le tableau alloué contenant toutes les cartes.
 */
Carte* genererCartes() {
	// Allouer de la mémoire pour le tableau de cartes
	Carte* cartes = malloc(TOTAL_CARTES * sizeof(Carte));
	if (!cartes) {
		perror("Erreur d'allocation mémoire");
		exit(EXIT_FAILURE);
	}

	int index = 0;
	for (int i = 0; i < 8; i++) {
		//cartes[index++] = (Carte){NOIR, PLUS_QUATRE};
		cartes[index++] = (Carte){NOIR, CHANGEMENT_COULEUR};
	}

	// Générer les cartes normales
	for (Couleur c = ROUGE; c <= VERT; c++) {
		for (Valeur v = ZERO; v <= CHANGEMENT_SENS; v++) {
			// Ajouter une carte pour chaque couleur et valeur
			cartes[index++] = (Carte){c, v};

			// Les cartes ZERO sont uniques, les autres sont en double
			if (v != ZERO) {
				cartes[index++] = (Carte){c, v};
			}
		}
	}

	return cartes; // Retourner le tableau
}

/**
 * Melange les cartes
 * @param cartes l'ensemble des cartes à mélanger
 * @param nbCartes taille de l'ensemble des cartes
 */
void melangerCartes(Carte* cartes, int nbCartes) {
	srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

	for (int i = nbCartes - 1; i > 0; i--) {
		int j = rand() % (i + 1); // Choisir un index aléatoire
		Carte temp = cartes[i];
		cartes[i] = cartes[j];
		cartes[j] = temp;
	}
}

/**
 * Fonction pour piocher une carte et l'ajouter à la main d'un joueur.
 * @param partie La partie contenant la pioche et les joueurs.
 * @param indexJoueur L'index du joueur qui pioche.
 */
void piocherCarte(Partie* partie, int indexJoueur) {
	if (partie->pioche == NULL || partie->nbCartesPioche == 0) {
		printf("Erreur : La pioche est vide ou n'existe pas.\n");
		return;
	}

	// Vérifier qu'il y a encore des cartes dans la pioche
	if (partie->nbCartesPioche == 0) {
		printf("Pioche vide\n");
		return;
	}

	// Ajouter la première carte de la pioche à la main du joueur
	Joueur* joueur = &partie->joueurs[indexJoueur];
	if (joueur->tailleMain < TAILLE_MAIN_MAX) {  // Assure-toi que la main ne dépasse pas 5 cartes
		joueur->main[joueur->tailleMain] = partie->pioche[0];  // Pioche la carte
		joueur->tailleMain++;  // Augmente la taille de la main
	}

	// Décaler les cartes restantes dans la pioche
	for (int i = 1; i < partie->nbCartesPioche; i++) {
		partie->pioche[i - 1] = partie->pioche[i];
	}

	// Mettre la dernière carte à -1 pour signifier qu'elle est vide
	partie->pioche[partie->nbCartesPioche - 1] = (Carte){-1, -1};  // Représente une carte vide
	partie->nbCartesPioche--;  // Réduit le nombre de cartes dans la pioche
}

/**
 * Vérifie si une carte est "vide"
 * utile pour le début de partie 
 * @param c carte à vérifier
 * 
 * @return 1 si vide sinon 0
 */
int isCarteVide(Carte c) {
	if ((int)c.Couleur == -1 && (int)c.Valeur == -1) {
		return 1;
	}
	return 0;
}

