#include "serialize.h"


void serialiserPartie(Partie p, char* chaine) {
    char buffer[256]; // Tampon pour formater les données temporaires

    // Initialisation de la chaîne
    sprintf(chaine, "[200:%d:%d:%d:[", p.nbJoueurs, p.currentPlayer, p.sens);
    debugprintf("[200:%d:%d:%d\n:[\n", p.nbJoueurs, p.currentPlayer, p.sens);

    for (int i = 0; i < p.nbJoueurs; i++) {
        // Ajout des informations de chaque joueur
        sprintf(buffer, "%d:%d:%d:[", p.joueurs[i].idSocket, p.joueurs[i].idJoueur, p.joueurs[i].tailleMain);
        strcat(chaine, buffer);

        debugprintf("%d:%d:%d:[", p.joueurs[i].idSocket, p.joueurs[i].idJoueur, p.joueurs[i].tailleMain);

        for (int j = 0; j < p.joueurs[i].tailleMain; j++) {
            // Ajout des informations des cartes dans la main du joueur
            if (j == p.joueurs[i].tailleMain - 1) {
                sprintf(buffer, "%d,%d", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
                debugprintf("%d,%d", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
            } else {
                sprintf(buffer, "%d,%d:", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
                debugprintf("%d,%d:", p.joueurs[i].main[j].Couleur, p.joueurs[i].main[j].Valeur);
            }
            strcat(chaine, buffer);
        }

        // Fermeture du tableau du joueur
        if (i == p.nbJoueurs - 1) {
            strcat(chaine, "]]:");
            debugprintf("]\n]:\n");
        } else {
            strcat(chaine, "],");
            debugprintf("],\n");
        }
    }

    sprintf(buffer,"%d:%d,%d:[", p.carteVisible.Couleur, p.carteVisible.Valeur, p.nbCartesPioche);
    strcat(chaine, buffer);
    debugprintf("%d:%d,%d\n:[\n", p.carteVisible.Couleur, p.carteVisible.Valeur, p.nbCartesPioche);

    for(int i=0;i<p.nbCartesPioche;i++){
        // Ajout des informations des cartes dans la pioche
        if (i == p.nbCartesPioche - 1) {
            sprintf(buffer, "%d,%d\n]:", p.pioche[i].Couleur, p.pioche[i].Valeur);
            debugprintf("%d,%d\n]:", p.pioche[i].Couleur, p.pioche[i].Valeur);
        } else {
            sprintf(buffer, "%d,%d:", p.pioche[i].Couleur, p.pioche[i].Valeur);
            debugprintf("%d,%d:", p.pioche[i].Couleur, p.pioche[i].Valeur);
        }
        strcat(chaine, buffer);
    }

    sprintf(buffer, "%d:%d]", p.estFinie, p.nbTours);
    strcat(chaine, buffer);

    debugprintf("%d:%d]", p.estFinie, p.nbTours);
}


void deserialiserPartie(const char* chaine, Partie* p) {
    const char* ptr = chaine; // Pointeur pour parcourir la chaîne
    //printf(chaine);
    // Lecture des premières informations (nbJoueurs, currentPlayer, sens)
    sscanf(ptr, "[200:%d:%d:%d:[", &p->nbJoueurs, &p->currentPlayer, &p->sens);

    // Avancer le pointeur après cette section
    ptr = strchr(ptr, '[') + 13;
    //printf("\naaaa\n%s\naaaa",ptr);

    // Lecture des joueurs
    for (int i = 0; i < p->nbJoueurs; i++) {
        int idSocket, idJoueur, tailleMain;
        sscanf(ptr, "%d:%d:%d:[", &idSocket, &idJoueur, &tailleMain);
        p->joueurs[i].idSocket = idSocket;
        p->joueurs[i].idJoueur = idJoueur;
        p->joueurs[i].tailleMain = tailleMain;

        // Avancer le pointeur après les informations du joueur
        ptr = strchr(ptr, '[') + 1;

        // Lecture des cartes dans la main du joueur
        for (int j = 0; j < tailleMain; j++) {
            int Couleur, Valeur;

            // Dernière carte dans la main (pas de `:` après)
            if (j == tailleMain - 1) {
                sscanf(ptr, "%d,%d", &Couleur, &Valeur);
            } else {
                sscanf(ptr, "%d,%d:", &Couleur, &Valeur);
                ptr = strchr(ptr, ':') + 1; // Avancer après le `:`
            }

            p->joueurs[i].main[j].Couleur = Couleur;
            p->joueurs[i].main[j].Valeur = Valeur;
        }

        // Avancer le pointeur après la fin du tableau du joueur
        if (i == p->nbJoueurs - 1) {
            ptr = strstr(ptr, "]]:") + 3; // Dernier joueur (fin avec "]]:")
        } else {
            ptr = strchr(ptr, ']') + 2; // Autres joueurs (fin avec "],")
        }
    }

    // Lecture de la carte visible et du nombre de cartes dans la pioche
    sscanf(ptr, "%d:%d,%d:[", (int*) &p->carteVisible.Couleur, (int*) &p->carteVisible.Valeur, &p->nbCartesPioche);

    // Avancer le pointeur après cette section
    ptr = strchr(ptr, '[') + 1;

    // Lecture des cartes dans la pioche
    for (int i = 0; i < p->nbCartesPioche; i++) {
        int Couleur, Valeur;

        // Dernière carte dans la pioche (pas de `:` après)
        if (i == p->nbCartesPioche - 1) {
            sscanf(ptr, "%d,%d\n]:", &Couleur, &Valeur);
        } else {
            sscanf(ptr, "%d,%d:", &Couleur, &Valeur);
            ptr = strchr(ptr, ':') + 1; // Avancer après le `:`
        }

        p->pioche[i].Couleur = Couleur;
        p->pioche[i].Valeur = Valeur;
    }

    // Lecture des informations finales (estFinie, nbTours)
    sscanf(ptr, "%d:%d]", &p->estFinie, &p->nbTours);
}

void debugprintf(const char* format, ...) {
    if (DEBUG) {
        va_list args;                // Initialise une liste d'arguments variadiques
        va_start(args, format);      // Associe la liste avec les arguments passés à la fonction
        vprintf(format, args);       // Appelle printf avec les arguments
        va_end(args);                // Termine l'utilisation des arguments variadiques
    }
}