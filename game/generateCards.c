#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "struct.h"
#include "enhanceTerminal.h"

void afficherCarte(Carte c);
Carte *genererCartes(int *nbCartes);
void melangerCartes(Carte *cartes, int nbCartes);

int main() {
    int nbCartes = 0;

    // Générer les cartes
    Carte *cartes = genererCartes(&nbCartes);

    // Initialiser la graine pour l'aléatoire
    srand(time(NULL));

    // Mélanger les cartes
    melangerCartes(cartes, nbCartes);

    // Afficher les cartes mélangées
    printf("Cartes mélangées :\n");
    for (int i = 0; i < nbCartes; i++) {
        afficherCarte(cartes[i]);
    }

    // Libérer la mémoire
    free(cartes);
    return 0;
}

/**
 * Génère toutes les cartes possibles dans un tableau alloué dynamiquement.
 * @param nbCartes Un pointeur pour stocker le nombre total de cartes générées.
 * @return Un pointeur vers le tableau alloué contenant toutes les cartes.
 */
Carte *genererCartes(int *nbCartes) {
    Carte *cartes = malloc(TOTAL_CARTES * sizeof(Carte)); // Allouer de la mémoire pour le tableau
    if (!cartes) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    int index = 0;

    // Générer les cartes normales
    for (Couleur c = ROUGE; c <= VERT; c++) {
        for (Valeur v = ZERO; v <= CHANGEMENT_SENS; v++) {
            // Ajouter une carte pour chaque couleur et valeur
            cartes[index++] = (Carte){c, v};

            // Les cartes ZERO sont uniques, les autres sont en double
            if (v != ZERO) {
                cartes[index++] = (Carte){c, v};
            }
        }
    }

    // Générer les cartes spéciales (NOIR : PLUS_QUATRE et CHANGEMENT_COULEUR)
    for (int i = 0; i < 4; i++) { // 4 exemplaires pour chaque carte spéciale
        cartes[index++] = (Carte){NOIR, PLUS_QUATRE};
        cartes[index++] = (Carte){NOIR, CHANGEMENT_COULEUR};
    }

    *nbCartes = index; // Stocker le total des cartes générées
    return cartes; // Retourner le tableau
}

/**
 * Affiche une carte donnée.
 * @param c La carte à afficher.
 */
void afficherCarte(Carte c) {
    const char *couleurs[] = {"ROUGE", "BLEU", "JAUNE", "VERT", "NOIR"};
    const char *valeurs[] = {
        "ZERO", "UN", "DEUX", "TROIS", "QUATRE", "CINQ", "SIX", "SEPT", "HUIT", "NEUF",
        "PLUS_DEUX", "PASSE_TOUR", "CHANGEMENT_SENS", "CHANGEMENT_COULEUR", "PLUS_QUATRE"
    };

    printf("[%s, %s]\n", couleurs[c.Couleur], valeurs[c.Valeur]);
}

void melangerCartes(Carte *cartes, int nbCartes) {
    for (int i = nbCartes - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Choisir un index aléatoire
        Carte temp = cartes[i];
        cartes[i] = cartes[j];
        cartes[j] = temp;
    }
}