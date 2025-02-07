#include "player.h"

int jouerPartieClient(Partie* partie,int idJoueur, socket_t socketHost){
    int input = 0;

    // Récupère l'id du joueur dans la partie
    int myID = idJoueur-1;

    /* Si c'est son tour, affiche la main avec action
       attend coup à faire
       envoie le coup

       Si c'est pas son tour, affiche sa main avec la carte actuelle
       attend que le host envoie un changement
    */
    while (!partie->estFinie) {
        clearScreen();
        if (partie->currentPlayer != myID) {
            printf("\nCarte visible : ");
            afficherCarte(partie->carteVisible);
            printf("Il affiche\n");
            afficherMain(partie->joueurs[myID]);
            printf("Il affiche main\n");

            //Arret ici
            resEnvoiPartie(socketHost, partie);
            printf("Il recup le coup\n");
        } else {
            afficherMainAvecSelection(partie->joueurs[partie->currentPlayer], partie->carteVisible);
            printf("Il affiche pour choisir\n");
            scanf("%d", &input);
            if (input == partie->joueurs[partie->currentPlayer].tailleMain) {
                piocherCarte(partie, partie->currentPlayer);
                prochainTour(partie);
                printf("Il fait son ptit truc\n");
                reqEnvoiPartieClient(socketHost, partie);
                printf("Il envoi le coup client\n");
            } else if (input < 0 || input >= partie->joueurs[partie->currentPlayer].tailleMain) {
                printf("Erreur : Entrée invalide.\n");
            } else {
                if (jouerCarte(partie, partie->currentPlayer, partie->joueurs[partie->currentPlayer].main[input])) {
                    if (partie->joueurs[partie->currentPlayer].tailleMain == 0) {
                        printf("Le joueur %d a gagné la partie en %d tours\n", partie->currentPlayer, partie->nbTours);
                        partie->estFinie = 1;
                    } else {
                        prochainTour(partie);
                    }
                    reqEnvoiPartieClient(socketHost, partie);
                }
            }
        }
    }
    return 0;
    
}

int jouerPartieServeur(Partie* partie, socket_t* sockets){
    int input;

    // Récupère l'id du joueur dans la partie
    int myID = 0;

    /* Si c'est son tour, affiche la main avec action
       attend coup à faire
       envoie le coup

       Si c'est pas son tour, affiche sa main avec la carte actuelle
       attend que le host envoie un changement
    */
    while (!partie->estFinie) {
        input=0;
        clearScreen();
        if (partie->currentPlayer != myID) {
            printf("\nCarte visible : ");
            afficherCarte(partie->carteVisible);
            afficherMain(partie->joueurs[myID]);
            printf("Il affiche\n");
            //resEnvoiCoup(partie->joueurs[partie->currentPlayer].idSocket, partie);
            resEnvoiPartie(sockets[partie->currentPlayer-1],partie);
            printf("Il recup coup du joueur\n");
            //reqEnvoiCoupServeur(sockets, partie);
            reqEnvoiPartie(sockets, partie);
            printf("Il transmet l'info\n");
        } else {
            afficherMainAvecSelection(partie->joueurs[partie->currentPlayer], partie->carteVisible);
            printf("Il affiche\n");
            scanf("%d", &input);
            //arrete ici
            if (input == partie->joueurs[partie->currentPlayer].tailleMain) {
                piocherCarte(partie, partie->currentPlayer);
                printf("Il a pioché\n");
                prochainTour(partie);
                printf("Il fait son ptit truc serveur\n");
                reqEnvoiPartie(sockets, partie);
                //reqEnvoiCoupServeur(sockets, partie);
                printf("Il envoi le coup serveur\n");

            } else if (input < 0 || input >= partie->joueurs[partie->currentPlayer].tailleMain) {
                printf("Erreur : Entrée invalide.\n");
            } else {
                if (jouerCarte(partie, partie->currentPlayer, partie->joueurs[partie->currentPlayer].main[input])) {
                    if (partie->joueurs[partie->currentPlayer].tailleMain == 0) {
                        printf("Le joueur %d a gagné la partie en %d tours\n", partie->currentPlayer, partie->nbTours);
                        partie->estFinie = 1;
                    } else {
                        prochainTour(partie);
                    }
                    reqEnvoiPartie(sockets, partie);
                }
            }
        }
    }
    return 0;
}
