#include "game.h"

//TODO changement de couleur et +4

int codes[100];
int indexCode=0;

/**
 * states :
 * 0 : Menu principal
 * 1 : Attente de partie publique
 * 2 : Entre le code de la partie privée
 * 3 : Attente de partie privée (hébergeur)
 * 4 : Attente de partie privée (rejoindre)
 * 
 */
int state = 0;

int main() {
    int input = 0;
    Partie partie = initPartie(4);
    
    for(int i=0; i<partie.nbJoueurs; i++){
        printf("\nJoueur %d\n",i+1);
        afficherMain(partie.joueurs[i]);
    }

    while(!partie.estFinie){
        clearScreen();
        afficherMainAvecSelection(partie.joueurs[partie.currentPlayer], partie.carteVisible);
        scanf("%d", &input);
        if (input == partie.joueurs[partie.currentPlayer].tailleMain) {
            piocherCarte(&partie, partie.currentPlayer);
            prochainTour(&partie);
        } else if (input < 0 || input >= partie.joueurs[partie.currentPlayer].tailleMain) {
            printf("Erreur : Entrée invalide.\n");
        } else{
            if(jouerCarte(&partie, partie.currentPlayer, partie.joueurs[partie.currentPlayer].main[input])){
                if(partie.joueurs[partie.currentPlayer].tailleMain == 0){
                    printf("Le joueur %d a gagné la partie en %d tours\n", partie.currentPlayer+1, partie.nbTours);
                    partie.estFinie = 1;
                }else{
                    prochainTour(&partie);
                }
                
            }
        }
        
    }
    
    return 0;
}

Partie initPartie(int nbJoueurs) {
    Partie partie;
    partie.estFinie = 0;
    partie.nbJoueurs = nbJoueurs;
    partie.sens = -1; 
    partie.nbTours = 0;
    Carte startCard = {-1, -1};  // Carte invalide
    partie.carteVisible= startCard;
    partie.currentPlayer = 3;

    // Initialiser les joueurs
    for(int i = 0; i < partie.nbJoueurs; i++) {
        Joueur joueur;
        joueur.idJoueur = i;
        joueur.tailleMain = 0; 
        joueur.main = malloc(TAILLE_MAIN_MAX * sizeof(Carte));  // Allouer de la mémoire pour la main

        if (joueur.main == NULL) {
            perror("Erreur d'allocation mémoire pour la main du joueur");
            exit(EXIT_FAILURE);
        }

        // Initialisation de la main à -1, -1
        for (int j = 0; j < TAILLE_MAIN_MAX; j++) {
            joueur.main[j].Couleur = -1;  // Valeur invalide
            joueur.main[j].Valeur = -1;   // Valeur invalide
        }

        partie.joueurs[i] = joueur;
    }

    // Générer et mélanger les cartes
    partie.pioche = genererCartes();
    partie.nbCartesPioche = TOTAL_CARTES;
    melangerCartes(partie.pioche, partie.nbCartesPioche);

    // Distribuer les cartes à chaque joueur
    for (int i = 0; i < partie.nbJoueurs; i++) {
        for (int j = 0; j < TAILLE_MAIN; j++) { 
            piocherCarte(&partie, i);   // La fonction piocherCarte ajoute la carte à la main du joueur
        }
    }

    return partie;
}


// Fonction pour jouer une carte
int jouerCarte(Partie *partie, int idJoueur, Carte carteJouee) {
    Joueur *joueur = &partie->joueurs[idJoueur];
    int carteTrouvee = 0;  // Pour vérifier si la carte est dans la main du joueur
    int indexCarteJouee = -1;


    // Vérifier si la carte est dans la main du joueur
    for (int i = 0; i < joueur->tailleMain; i++) {
        if ( joueur->main[i].Couleur == carteJouee.Couleur && joueur->main[i].Valeur == carteJouee.Valeur ) {
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

    if(carteJouee.Valeur == PLUS_DEUX){
        printf("Le joueur %d pioche 2 cartes\n", ((idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs)+1);
        piocherCarte(partie, (idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs);
        piocherCarte(partie, (idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs);
    }
    if(carteJouee.Valeur == PASSE_TOUR){
        printf("Le joueur %d passe son tour\n", ((idJoueur + partie->sens + partie->nbJoueurs) % partie->nbJoueurs)+1);
        prochainTour(partie);
    }
    if(carteJouee.Valeur == CHANGEMENT_SENS){
        printf("Changement de sens\n");
        partie->sens = -partie->sens;
    }

    afficherCarte(partie->carteVisible);
    return 1;
}


int generateCode(){
    int isHereFlag=0;

    srand(time(NULL));
    int code = rand() % 10000;

    for(int i = 0; i < 100; i++){
        if(codes[i] == code){
            isHereFlag=1;
            break;
        }
    }
    if(isHereFlag == 0){
        codes[indexCode] = code;
        indexCode++;
        return code;
    }else{
        return generateCode();
    }
}

void prochainTour(Partie *partie){
    partie->currentPlayer = (partie->currentPlayer + partie->sens + partie->nbJoueurs) % partie->nbJoueurs;
    partie->nbTours++;
}