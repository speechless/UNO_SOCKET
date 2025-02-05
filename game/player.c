#include "game.h"

int main() {
    int input = 0;

    //récupère l'id du joueur dans la partie
    int myId = getID();


    /*Si c'est son tour, affiche la main avec action
    attend coup à faire
    envoie le coup

    Si c'est pas son tour, affiche sa main avec la carte actuelle
    attend que le host envoie un changement
    */
    while(!partie.estFinie){
        clearScreen();
        if(partie.currentPlayer != myID){
            printf("\nCarte visible : ");
            afficherCarte(p.carteVisible);
            afficherMain(partie.joueurs[i]);
            resReceptionCoup(/*partie.joueurs[partie.currentPlayer].idSocket,*/ partie);
        }else{
            afficherMainAvecSelection(partie.joueurs[partie.currentPlayer], partie.carteVisible);
            scanf("%d", &input);
            if (input == partie.joueurs[partie.currentPlayer].tailleMain) {
                piocherCarte(&partie, partie.currentPlayer);
                prochainTour(&partie);

                reqEnvoiCoup(/*partie.joueurs[partie.currentPlayer].idSocket,*/ partie);


            } else if (input < 0 || input >= partie.joueurs[partie.currentPlayer].tailleMain) {
                printf("Erreur : Entrée invalide.\n");
            } else{
                if(jouerCarte(&partie, partie.currentPlayer, partie.joueurs[partie.currentPlayer].main[input])){
                    if(partie.joueurs[partie.currentPlayer].tailleMain == 0){
                        printf("Le joueur %d a gagné la partie en %d tours\n", partie.currentPlayer, partie.nbTours);
                        partie.estFinie = 1;
                    }else{
                        prochainTour(&partie);
                    }
                    reqEnvoiCoup(/*partie.joueurs[partie.currentPlayer].idSocket,*/ partie);
                }
            }
        }
        
       
    }
    return 0;
}

int getID(){}