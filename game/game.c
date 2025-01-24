#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "struct.h"
#include "enhanceTerminal.h"

void afficherMenu(int state);
void afficherMain(Joueur j);

Carte 


int main() {
    int input = 0;

    while (1) {
        clearScreen();  // Effacer l'écran après l'input
        afficherMenu(input);
        
        // Lecture de l'entrée utilisateur
        if (scanf("%d", &input) != 1) {  
            // Si l'utilisateur n'entre pas un entier
            printf("Entrée invalide. Veuillez entrer un nombre.\n");
            while (getchar() != '\n');
            continue;
        }

        // Si vous voulez ajouter des conditions pour l'input
        printf("\nVous avez entré : %d\n", input);
    }

    return 0;
}

void afficherMenu(int state) {

    switch (state)
    {
    case 0:
        setTerm(BLACK);
        printf("===== Menu Principal =====\n");
        resetTerm();
        printf("1. Lancer une partie publique\n");
        printf("2. Lancer une partie privée\n");
        printf("3. Héberger une partie privée\n");
        printf("4. Quitter\n");
        setTerm(BLACK);
        printf("==========================\n");
        resetTerm();
        printf("\nInput : ");
        break;

    case 1:
        printf("\nInput : ");
        break;
    
    default:
        break;
    }

    
}

void afficherMain(Joueur j){
    
}