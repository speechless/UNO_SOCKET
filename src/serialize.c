#include "serialize.h"
#include <common.h>

int STATUS_CODE_SERIA_PARTIE = 200;
int STATUS_CODE_SERIA_COUP = 300;

void serialiserPartie(Partie* p, char* chaine) {
	char buffer[500]; // Tampon pour formater les données temporaires
	// Initialisation de la chaîne
	sprintf(chaine, "[%d:%d:%d:%d:[", STATUS_CODE_SERIA_PARTIE, p->nbJoueurs, p->currentPlayer, p->sens);
	//debugprintf("[%d:%d:%d:%d\n:[\n",STATUS_CODE_SERIA_PARTIE, p.nbJoueurs, p.currentPlayer, p.sens);

	for (int i = 0; i < p->nbJoueurs; i++) {
		// Ajout des informations de chaque joueur
		//sauf la socket

		sprintf(buffer, "%d:%d:[", p->joueurs[i].idJoueur, p->joueurs[i].tailleMain);
		strcat(chaine, buffer);

		debugprintf("%d:%d:[", p->joueurs[i].idJoueur, p->joueurs[i].tailleMain);

		for (int j = 0; j < p->joueurs[i].tailleMain; j++) {
			// Ajout des informations des cartes dans la main du joueur
			if (j == p->joueurs[i].tailleMain - 1) {
				sprintf(buffer, "%d,%d", p->joueurs[i].main[j].Couleur, p->joueurs[i].main[j].Valeur);
				debugprintf("%d,%d", p->joueurs[i].main[j].Couleur, p->joueurs[i].main[j].Valeur);
			}
			else {
				sprintf(buffer, "%d,%d:", p->joueurs[i].main[j].Couleur, p->joueurs[i].main[j].Valeur);
				debugprintf("%d,%d:", p->joueurs[i].main[j].Couleur, p->joueurs[i].main[j].Valeur);
			}
			strcat(chaine, buffer);
		}

		// Fermeture du tableau du joueur
		if (i == p->nbJoueurs - 1) {
			strcat(chaine, "]]:");
			debugprintf("]\n]:\n");
		}
		else {
			strcat(chaine, "],");
			debugprintf("],\n");
		}
	}

	sprintf(buffer, "%d:%d,%d:[", p->carteVisible.Couleur, p->carteVisible.Valeur, p->nbCartesPioche);
	strcat(chaine, buffer);
	debugprintf("%d:%d,%d\n:[\n", p->carteVisible.Couleur, p->carteVisible.Valeur, p->nbCartesPioche);

	for (int i = 0;i < p->nbCartesPioche;i++) {
		// Ajout des informations des cartes dans la pioche
		if (i == p->nbCartesPioche - 1) {
			sprintf(buffer, "%d,%d]:", p->pioche[i].Couleur, p->pioche[i].Valeur);
			debugprintf("%d,%d\n]:", p->pioche[i].Couleur, p->pioche[i].Valeur);
		}
		else {
			sprintf(buffer, "%d,%d:", p->pioche[i].Couleur, p->pioche[i].Valeur);
			debugprintf("%d,%d:", p->pioche[i].Couleur, p->pioche[i].Valeur);
		}
		strcat(chaine, buffer);
	}

	sprintf(buffer, "%d]", p->estFinie);
	strcat(chaine, buffer);

	debugprintf("######PARTIE SERIALISEE######\n%s\n######################\n", chaine);
}


// Fonction pour extraire un entier d'une chaîne
int extraireEntier(char** str) {
	char* end;
	int valeur = strtol(*str, &end, 10);
	if (*end != ':' && *end != ',' && *end != ']') {
		debugprintf("Erreur de format : délimiteur attendu.\n");
		exit(EXIT_FAILURE);
	}
	*str = (*end == ':' || *end == ',') ? end + 1 : end;
	return valeur;
}

// Fonction pour extraire une carte d'une chaîne
Carte extraireCarte(char** str) {
	Carte carte;
	carte.Couleur = extraireEntier(str); // Extraire la couleur
	carte.Valeur = extraireEntier(str);  // Extraire la valeur
	return carte;
}

// Fonction de désérialisation
void deserialiserPartie(char* chaine, Partie* p) {
	char* str = strdup(chaine); // Copie de la chaîne pour éviter de la modifier
	char* ptr = str;

	// Ignorer le premier caractère '['
	if (*ptr == '[') ptr++;

	// Lire les informations de base de la partie
	/*int statusCode = */extraireEntier(&ptr); // Code de statut (200)
	p->nbJoueurs = extraireEntier(&ptr);   // Nombre de joueurs
	p->currentPlayer = extraireEntier(&ptr); // Joueur actuel
	p->sens = extraireEntier(&ptr);        // Sens du jeu

	/*// Allouer de la mémoire pour les joueurs
	for (int i = 0; i < p->nbJoueurs; i++) {
		p->joueurs[i].main = (Carte*)malloc(TAILLE_MAIN_MAX * sizeof(Carte));
	}
	if (p->joueurs == NULL) {
		fprintf(stderr, "Erreur d'allocation mémoire pour les joueurs.\n");
		free(str);
		return;
	}*/

	// Ignorer le caractère '['
	if (*ptr == '[') ptr++;

	// Lire les informations des joueurs
	for (int i = 0; i < p->nbJoueurs; i++) {
		p->joueurs[i].idJoueur = extraireEntier(&ptr); // ID du joueur
		p->joueurs[i].tailleMain = extraireEntier(&ptr); // Taille de la main

		// Allouer de la mémoire pour la main du joueur
		p->joueurs[i].main = (Carte*)malloc(TAILLE_MAIN_MAX * sizeof(Carte));
		if (p->joueurs[i].main == NULL) {
			debugprintf("Erreur d'allocation mémoire pour la main du joueur.\n");
			free(str);
			return;
		}

		// Ignorer le caractère '['
		if (*ptr == '[') ptr++;

		// Lire les cartes de la main du joueur
		for (int j = 0; j < p->joueurs[i].tailleMain; j++) {
			p->joueurs[i].main[j] = extraireCarte(&ptr); // Extraire une carte
			if (*ptr == ',') ptr++; // Passer le délimiteur entre les cartes
		}

		// Ignorer les caractères de fin de joueur
		if (*ptr == ']') ptr++;
		if (*ptr == ']') ptr++;
		if (*ptr == ':') ptr++;
		if (*ptr == ',') ptr++;
	}

	//printf("Chaine restante : %s\n", ptr);
	// Lire la carte visible et le nombre de cartes dans la pioche
	p->carteVisible = extraireCarte(&ptr); // Carte visible
	p->nbCartesPioche = extraireEntier(&ptr); // Nombre de cartes dans la pioche

	// Allouer de la mémoire pour la pioche
	p->pioche = (Carte*)malloc(TAILLE_MAIN_MAX * sizeof(Carte));
	if (p->pioche == NULL) {
		debugprintf("Erreur d'allocation mémoire pour la pioche.\n");
		free(str);
		return;
	}

	// Ignorer le caractère '['
	if (*ptr == '[') ptr++;

	// Lire les cartes de la pioche
	for (int i = 0; i < p->nbCartesPioche; i++) {
		p->pioche[i] = extraireCarte(&ptr); // Extraire une carte
		if (*ptr == ',') ptr++; // Passer le délimiteur entre les cartes
	}

	// Ignorer le caractère ']' et ':'
	if (*ptr == ']') ptr++;
	if (*ptr == ':') ptr++;

	debugprintf("Chaine restante : ######%s#########\n", ptr);

	// Lire les informations de fin de partie
	p->estFinie = extraireEntier(&ptr); // Partie terminée ?

	debugprintf("Partie finie : %d\n", p->estFinie);
	// Libérer la copie de la chaîne
	free(str);

	//printf("\n%s\n",chaine);
}

void serialiserCoup(Partie p, char* chaine) {
	char buffer[256]; // Tampon pour formater les données temporaires

	// Initialisation de la chaîne
	sprintf(chaine, "[%d:%d:%d:[", STATUS_CODE_SERIA_COUP, p.currentPlayer, p.sens);
	debugprintf("[%d:%d:%d\n:[\n", STATUS_CODE_SERIA_COUP, p.currentPlayer, p.sens);

	// Ajout des informations de chaque joueur
	for (int i = 0; i < p.nbJoueurs; i++) {
		sprintf(buffer, "%d:%d:[", p.joueurs[i].idJoueur, p.joueurs[i].tailleMain);
		strcat(chaine, buffer);

		debugprintf("%d:%d:[", p.joueurs[i].idJoueur, p.joueurs[i].tailleMain);

		// Ajout des informations des cartes dans la main du joueur
		for (int j = 0; j < p.joueurs[i].tailleMain; j++) {
			if (j == p.joueurs[i].tailleMain - 1) {
				sprintf(buffer, "%d,%d", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
				debugprintf("%d,%d", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
			}
			else {
				sprintf(buffer, "%d,%d:", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
				debugprintf("%d,%d:", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
			}
			strcat(chaine, buffer);
		}

		// Fermeture du tableau du joueur
		if (i == p.nbJoueurs - 1) {
			strcat(chaine, "]]:");
			debugprintf("]\n]:\n");
		}
		else {
			strcat(chaine, "],");
			debugprintf("],\n");
		}
	}

	// Ajout de la carte visible et du nombre de cartes dans la pioche
	sprintf(buffer, "%d:%d,%d:[", p.carteVisible.Couleur, p.carteVisible.Valeur, p.nbCartesPioche);
	strcat(chaine, buffer);
	debugprintf("%d:%d,%d\n:[\n", p.carteVisible.Couleur, p.carteVisible.Valeur, p.nbCartesPioche);

	// Ajout des informations des cartes dans la pioche
	for (int i = 0; i < p.nbCartesPioche; i++) {
		if (i == p.nbCartesPioche - 1) {
			sprintf(buffer, "%d,%d\n]:", p.pioche[i].Couleur, p.pioche[i].Valeur);
			debugprintf("%d,%d\n]:", p.pioche[i].Couleur, p.pioche[i].Valeur);
		}
		else {
			sprintf(buffer, "%d,%d:", p.pioche[i].Couleur, p.pioche[i].Valeur);
			debugprintf("%d,%d:", p.pioche[i].Couleur, p.pioche[i].Valeur);
		}
		strcat(chaine, buffer);
	}

	// Ajout des informations finales (estFinie)
	sprintf(buffer, "%d]", p.estFinie);
	strcat(chaine, buffer);

	debugprintf(":%d]", p.estFinie);
}


void deserialiserCoup(char* chaine, Partie* p) {
	char* str = strdup(chaine); // Copie de la chaîne pour éviter de la modifier
	char* ptr = str;

	// Ignorer le premier caractère '['
	if (*ptr == '[') ptr++;

	// Lire les informations de base du coup
	/*int statusCode = */extraireEntier(&ptr); // Code de statut (STATUS_CODE_SERIA_COUP)
	p->currentPlayer = extraireEntier(&ptr); // Joueur actuel
	p->sens = extraireEntier(&ptr); // Sens du jeu

	// Ignorer le caractère '['
	if (*ptr == '[') ptr++;

	// Lire les informations des joueurs
	for (int i = 0; i < p->nbJoueurs; i++) {
		if (i == p->currentPlayer || (p->currentPlayer + p->sens + p->nbJoueurs) % p->nbJoueurs) {
			p->joueurs[i].idJoueur = extraireEntier(&ptr); // ID du joueur
			p->joueurs[i].tailleMain = extraireEntier(&ptr); // Taille de la main

			// Allouer de la mémoire pour la main du joueur
			p->joueurs[i].main = (Carte*)malloc(TAILLE_MAIN_MAX * sizeof(Carte));
			if (p->joueurs[i].main == NULL) {
				debugprintf("Erreur d'allocation mémoire pour la main du joueur.\n");
				free(str);
				return;
			}

			// Ignorer le caractère '['
			if (*ptr == '[') ptr++;

			// Lire les cartes de la main du joueur
			for (int j = 0; j < p->joueurs[i].tailleMain; j++) {
				p->joueurs[i].main[j] = extraireCarte(&ptr); // Extraire une carte
				if (*ptr == ',') ptr++; // Passer le délimiteur entre les cartes
			}

			// Ignorer les caractères de fin de joueur
			if (*ptr == ']') ptr++;
			if (*ptr == ',') ptr++;
		}
	}

	// Lire la carte visible et le nombre de cartes dans la pioche
	p->carteVisible = extraireCarte(&ptr); // Carte visible
	p->nbCartesPioche = extraireEntier(&ptr); // Nombre de cartes dans la pioche

	// Allouer de la mémoire pour la pioche
	p->pioche = (Carte*)malloc(TAILLE_MAIN_MAX * sizeof(Carte));
	if (p->pioche == NULL) {
		debugprintf("Erreur d'allocation mémoire pour la pioche.\n");
		free(str);
		return;
	}

	// Ignorer le caractère '['
	if (*ptr == '[') ptr++;

	// Lire les cartes de la pioche
	for (int i = 0; i < p->nbCartesPioche; i++) {
		p->pioche[i] = extraireCarte(&ptr); // Extraire une carte
		if (*ptr == ',') ptr++; // Passer le délimiteur entre les cartes
	}

	// Ignorer le caractère ']'
	if (*ptr == ']') ptr++;

	// Lire les informations finales (estFinie)
	p->estFinie = extraireEntier(&ptr); // Partie terminée ?

	// Libérer la copie de la chaîne
	free(str);
}

