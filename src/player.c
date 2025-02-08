#include "player.h"
#include <common.h>

int jouerPartieClient(Partie* partie, int idJoueur, socket_t socketHost) {
	int input;
	debugprintf("idJoueur = %d, idCourant = %d\n", idJoueur, partie->currentPlayer);
	int indexJoueurLocal = getIndexFromIdJoueur(idJoueur, partie->joueurs, partie->nbJoueurs);

	/* Si c'est son tour, affiche la main avec action
	   attend coup à faire
	   envoie le coup

	   Si c'est pas son tour, affiche sa main avec la carte actuelle
	   attend que le host envoie un changement
	*/
	while (!partie->estFinie) {
		input = 0;
#ifndef DEBUG
		clearScreen();
#endif
		if (partie->currentPlayer != idJoueur) {
			printf("\nCarte visible : ");
			afficherCarte(partie->carteVisible);
			printf("\nMain du joueur %d : \n", idJoueur);
			afficherMain(partie->joueurs[indexJoueurLocal]);
			resEnvoiPartie(socketHost, partie);
		}
		else {
			afficherMainAvecSelection(partie->joueurs[indexJoueurLocal], partie->carteVisible);
			scanf("%d", &input);
			if (input == partie->joueurs[indexJoueurLocal].tailleMain) {
				piocherCarte(partie, indexJoueurLocal);
				prochainTour(partie);
				reqEnvoiPartieClient(socketHost, partie);
				resEnvoiPartie(socketHost, partie);
			}
			else if (input < 0 || input >= partie->joueurs[indexJoueurLocal].tailleMain) {
				printf("Erreur : Entrée invalide.\n");
			}
			else {
				if (jouerCarte(partie, partie->currentPlayer, partie->joueurs[indexJoueurLocal].main[input])) {
					if (partie->joueurs[indexJoueurLocal].tailleMain == 0) {
						printf("Le joueur %d a gagné la partie !!\n", partie->currentPlayer);
						partie->estFinie = 1;
						reqEnvoiPartieClient(socketHost, partie);
						break;
					}
					else {
						prochainTour(partie);
					}
					reqEnvoiPartieClient(socketHost, partie);
					resEnvoiPartie(socketHost, partie);
				}
				else {
					debugprintf(" une erreur est survenue\n");
				}
			}
		}
	}
	return 0;

}

int jouerPartieServeur(Partie* partie, int idJoueur, client_t* clients) {
	int input;


	int indexJoueurLocal = getIndexFromIdJoueur(idJoueur, partie->joueurs, partie->nbJoueurs);

	/* Si c'est son tour, affiche la main avec action
	   attend coup à faire
	   envoie le coup

	   Si c'est pas son tour, affiche sa main avec la carte actuelle
	   attend que le host envoie un changement
	*/
	while (!partie->estFinie) {
		debugprintf("idJoueur = %d, idCourant = %d\n", idJoueur, partie->currentPlayer);
		for (int i = 0; i < partie->nbJoueurs; i++) {
			debugprintf("client %d indice %d connecté sur port %d\n", clients[i].id, i, clients[i].port);
		}
		for (int i = 0; i < partie->nbJoueurs; i++) {
			debugprintf("joueur %d indice %d\n", partie->joueurs[i].idJoueur, i);
		}

		input = 0;
#ifndef DEBUG
		clearScreen();
#endif
		if (partie->currentPlayer != idJoueur) {
			printf("\nCarte visible : ");
			afficherCarte(partie->carteVisible);
			printf("\nMain du joueur %d : \n", idJoueur);
			afficherMain(partie->joueurs[indexJoueurLocal]);
			int indexJoueurActuel = getIndexFromIdClient(partie->currentPlayer, clients, partie->nbJoueurs);
			resEnvoiPartie(clients[indexJoueurActuel].socket, partie);
			reqEnvoiPartie(clients, partie);
		}
		else {
			afficherMainAvecSelection(partie->joueurs[indexJoueurLocal], partie->carteVisible);
			scanf("%d", &input);
			if (input == partie->joueurs[indexJoueurLocal].tailleMain) {
				piocherCarte(partie, indexJoueurLocal);
				prochainTour(partie);
				reqEnvoiPartie(clients, partie);

			}
			else if (input < 0 || input >= partie->joueurs[indexJoueurLocal].tailleMain) {
				debugprintf("Erreur : Entrée invalide.\n");
			}
			else {
				if (jouerCarte(partie, partie->currentPlayer, partie->joueurs[indexJoueurLocal].main[input])) {
					if (partie->joueurs[indexJoueurLocal].tailleMain == 0) {
						printf("Le joueur %d a gagné la partie !!\n", partie->currentPlayer);
						partie->estFinie = 1;
						reqEnvoiPartie(clients, partie);
					}
					else {
						prochainTour(partie);
					}
					reqEnvoiPartie(clients, partie);
				}
				else {
					debugprintf(" une erreur est survenue\n");
				}
			}
		}
	}
	if (partie->estFinie && partie->currentPlayer != idJoueur) {
		printf("Le joueur %d a gagné la partie !!\n", partie->currentPlayer);
	}
	return 0;
}
