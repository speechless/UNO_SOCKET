#include "player.h"
#include <common.h>

int jouerPartieClient(Partie* partie, int idJoueur, socket_t socketHost) {
	int input = 0;

	int indexJoueurLocal = getIndexFromIdJoueur(idJoueur, partie->joueurs, partie->nbJoueurs);

	/* Si c'est son tour, affiche la main avec action
	   attend coup à faire
	   envoie le coup

	   Si c'est pas son tour, affiche sa main avec la carte actuelle
	   attend que le host envoie un changement
	*/
	while (!partie->estFinie) {
		clearScreen();
		if (partie->currentPlayer != idJoueur) {
			printf("\nCarte visible : ");
			afficherCarte(partie->carteVisible);
			//printf("Il affiche\n");
			afficherMain(partie->joueurs[indexJoueurLocal]);
			//printf("Il affiche main\n");
		}
		else {

			afficherMainAvecSelection(partie->joueurs[indexJoueurLocal], partie->carteVisible);
			//printf("Il affiche pour choisir\n");
			scanf("%d", &input);
			if (input == partie->joueurs[indexJoueurLocal].tailleMain) {
				piocherCarte(partie, indexJoueurLocal);
				prochainTour(partie);
				//printf("Il fait son ptit truc\n");
				reqEnvoiPartieClient(socketHost, partie);
				//printf("Il envoi le coup client\n");
			}
			else if (input < 0 || input >= partie->joueurs[indexJoueurLocal].tailleMain) {
				printf("Erreur : Entrée invalide.\n");
			}
			else {
				if (jouerCarte(partie, partie->currentPlayer, partie->joueurs[indexJoueurLocal].main[input])) {
					if (partie->joueurs[indexJoueurLocal].tailleMain == 0) {
						printf("Le joueur %d a gagné la partie !!\n", partie->currentPlayer);
						partie->estFinie = 1;
					}
					else {
						prochainTour(partie);
					}
					reqEnvoiPartieClient(socketHost, partie);
				}
			}
		}

		resEnvoiPartie(socketHost, partie);
	}
	return 0;

}

int jouerPartieServeur(Partie* partie, int idJoueur, client_t* clients) {
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
		clearScreen();
		if (partie->currentPlayer != idJoueur) {
			debugprintf("\nCarte visible : ");
			afficherCarte(partie->carteVisible);

			afficherMain(partie->joueurs[indexJoueurLocal]);
			//printf("Il affiche\n");
			//resEnvoiCoup(partie->joueurs[partie->currentPlayer].idSocket, partie);
			int indexJoueurActuel = getIndexFromIdClient(partie->currentPlayer, clients, partie->nbJoueurs);
			resEnvoiPartie(clients[indexJoueurActuel].socket, partie);
			//printf("Il recup coup du joueur\n");
			//reqEnvoiCoupServeur(sockets, partie);
			reqEnvoiPartie(clients, partie);
			//printf("Il transmet l'info\n");
		}
		else {
			afficherMainAvecSelection(partie->joueurs[indexJoueurLocal], partie->carteVisible);
			//printf("Il affiche\n");
			scanf("%d", &input);
			if (input == partie->joueurs[indexJoueurLocal].tailleMain) {
				piocherCarte(partie, indexJoueurLocal);
				//printf("Il a pioché\n");
				prochainTour(partie);
				//printf("Il fait son ptit truc serveur\n");
				reqEnvoiPartie(clients, partie);
				//reqEnvoiCoupServeur(sockets, partie);
				//printf("Il envoi le coup serveur\n");

			}
			else if (input < 0 || input >= partie->joueurs[indexJoueurLocal].tailleMain) {
				printf("Erreur : Entrée invalide.\n");
			}
			else {
				if (jouerCarte(partie, partie->currentPlayer, partie->joueurs[indexJoueurLocal].main[input])) {
					if (partie->joueurs[indexJoueurLocal].tailleMain == 0) {
						printf("Le joueur %d a gagné la partie !!\n", partie->currentPlayer);
						partie->estFinie = 1;
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
	return 0;
}
