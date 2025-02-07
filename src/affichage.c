#include "affichage.h"


/**
 * Affiche une carte donnée.
 * @param c La carte à afficher.
 */
void afficherCarte(Carte c) {
	const char* couleurs[] = {"ROUGE", "BLEU", "JAUNE", "VERT", "NOIR"};
	const char* valeurs[] = {
		"ZERO", "UN", "DEUX", "TROIS", "QUATRE", "CINQ", "SIX", "SEPT", "HUIT", "NEUF",
		"PLUS_DEUX", "PASSE_TOUR", "CHANGEMENT_SENS", "CHANGEMENT_COULEUR", "PLUS_QUATRE"
	};

	if (!isCarteVide(c)) {
		switch (c.Couleur) {
			case ROUGE:
				setTerm(RED);
				break;
			case BLEU:
				setTerm(BLUE);
				break;
			case JAUNE:
				setTerm(YELLOW);
				break;
			case VERT:
				setTerm(GREEN);
				break;
			case NOIR:
				setTerm(BLACK);
				break;

			default:
				break;
		}

		printf("[%s, %s]\n", couleurs[c.Couleur], valeurs[c.Valeur]);
		resetTerm();
	}
	else {
		printf("Carte vide\n");
	}

}

void afficherCarteMain(Carte c, int index) {
	printf("%d - ", index);
	afficherCarte(c);
}

// Fonction pour afficher la main du joueur avec un choix
void afficherMainAvecSelection(Joueur joueur, Carte carteVisible) {
	// Affichage de la carte visible
	printf("\nCarte visible : ");
	afficherCarte(carteVisible);

	// Affichage de la main du joueur
	printf("Main du joueur %d :\n", joueur.idJoueur);
	for (int i = 0; i < joueur.tailleMain; i++) {
		afficherCarteMain(joueur.main[i], i);
	}
	printf("%d - Piocher une carte\n", joueur.tailleMain);

}


void afficherMain(Joueur j) {
	for (int i = 0; i < j.tailleMain; i++) {
		afficherCarte(j.main[i]);
	}
}