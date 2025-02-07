#include "game.h"

#define REAL 1
#define TEST 1

//TODO changement de couleur et +4


void initPartie(Partie* partie, int nbJoueurs, socket_t* sockets) {
	if (partie == NULL) {
		perror("Erreur d'allocation mémoire pour la partie");
		exit(EXIT_FAILURE);
	}

	partie->estFinie = 0;
	partie->nbJoueurs = nbJoueurs;
	partie->sens = 1;
	Carte startCard = {-1, -1};  // Carte invalide
	partie->carteVisible = startCard;
	partie->currentPlayer = 0;

	// Initialiser les joueurs
	for (int i = 0; i < partie->nbJoueurs; i++) {
		partie->joueurs[i].idJoueur = i;
		partie->joueurs[i].idSocket = sockets[i];
		partie->joueurs[i].tailleMain = 0;
		partie->joueurs[i].main = malloc(TAILLE_MAIN_MAX * sizeof(Carte));  // Allouer mémoire pour la main

		if (partie->joueurs[i].main == NULL) {
			perror("Erreur d'allocation mémoire pour la main du joueur");
			free(partie);
			exit(EXIT_FAILURE);
		}

		// Initialisation de la main à -1, -1
		for (int j = 0; j < TAILLE_MAIN_MAX; j++) {
			partie->joueurs[i].main[j].Couleur = -1;
			partie->joueurs[i].main[j].Valeur = -1;
		}
	}

	// Générer et mélanger les cartes
	partie->pioche = genererCartes();
	partie->nbCartesPioche = TOTAL_CARTES;
	melangerCartes(partie->pioche, partie->nbCartesPioche);

	// Distribuer les cartes aux joueurs
	for (int i = 0; i < partie->nbJoueurs; i++) {
		for (int j = 0; j < TAILLE_MAIN; j++) {
			piocherCarte(partie, i);   // La fonction piocherCarte ajoute la carte à la main du joueur
		}
	}

}

void initPartieClient(Partie* partie, int nbJoueurs) {
	if (partie == NULL) {
		perror("Erreur d'allocation mémoire pour la partie");
		exit(EXIT_FAILURE);
	}

	partie->estFinie = 0;
	partie->nbJoueurs = nbJoueurs;
	partie->sens = 1;
	Carte startCard = {-1, -1};  // Carte invalide
	partie->carteVisible = startCard;
	partie->currentPlayer = 0;

	// Initialiser les joueurs
	for (int i = 0; i < partie->nbJoueurs; i++) {
		partie->joueurs[i].idJoueur = i;
		partie->joueurs[i].tailleMain = 0;
		partie->joueurs[i].main = malloc(TAILLE_MAIN_MAX * sizeof(Carte));  // Allouer mémoire pour la main

		if (partie->joueurs[i].main == NULL) {
			perror("Erreur d'allocation mémoire pour la main du joueur");
			free(partie);
			exit(EXIT_FAILURE);
		}

		// Initialisation de la main à -1, -1
		for (int j = 0; j < TAILLE_MAIN_MAX; j++) {
			partie->joueurs[i].main[j].Couleur = -1;
			partie->joueurs[i].main[j].Valeur = -1;
		}
	}
}



// Fonction pour jouer une carte
int jouerCarte(Partie* partie, int idJoueur, Carte carteJouee) {

	Joueur* joueur = &partie->joueurs[idJoueur];
	int carteTrouvee = 0;  // Pour vérifier si la carte est dans la main du joueur
	int indexCarteJouee = -1;


	// Vérifier si la carte est dans la main du joueur
	for (int i = 0; i < joueur->tailleMain; i++) {
		if (joueur->main[i].Couleur == carteJouee.Couleur && joueur->main[i].Valeur == carteJouee.Valeur) {
			carteTrouvee = 1;
			indexCarteJouee = i;
			break;
		}
	}

	if (!carteTrouvee) {
		printf("Erreur : La carte n'est pas dans la main du joueur.\n");
		return 0;
	}

	// Vérifier si la carte est valide (couleur ou valeur égale à la carte visible)
	if ((carteJouee.Couleur != partie->carteVisible.Couleur
		&& carteJouee.Valeur != partie->carteVisible.Valeur
		&& carteJouee.Couleur != NOIR) && !isCarteVide(partie->carteVisible)) {
		printf("Erreur : La carte jouée n'est pas valide (pas la même couleur ou valeur).\n");
		return 0;
	}

	// Si la carte est valide et trouvée dans la main, on la joue
	// Déplacer la carte jouée dans la carte visible
	partie->carteVisible = carteJouee;

	// Enlever la carte de la main du joueur en décalant les cartes
	for (int i = indexCarteJouee; i < joueur->tailleMain - 1; i++) {
		joueur->main[i] = joueur->main[i + 1];
	}

	// Réduire la taille de la main du joueur
	joueur->tailleMain--;

	// Ajouter la carte visible au fond de la pioche
	if (partie->nbCartesPioche < 100) {  // Assurez-vous qu'il y a de l'espace dans la pioche
		partie->pioche[partie->nbCartesPioche] = partie->carteVisible;
		partie->nbCartesPioche++;
	}

	if (carteJouee.Valeur == PLUS_DEUX) {
		printf("Le joueur %d pioche 2 cartes\n", (idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs);
		piocherCarte(partie, (idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs);
		piocherCarte(partie, (idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs);
	}
	if (carteJouee.Valeur == PASSE_TOUR) {
		printf("Le joueur %d passe son tour\n", (idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs);
		prochainTour(partie);
	}
	if (carteJouee.Valeur == CHANGEMENT_SENS) {
		printf("Changement de sens\n");
		partie->sens = -partie->sens;
	}

	afficherCarte(partie->carteVisible);

	return 1;
}


void prochainTour(Partie* partie) {
	partie->currentPlayer = (partie->currentPlayer + partie->sens + partie->nbJoueurs) % partie->nbJoueurs;
}