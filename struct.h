#include <stdio.h>

typedef enum{
    ROUGE,
    BLEU,
    JAUNE,
    VERT
} Couleur;

typedef enum{
    ZERO,
    UN,
    DEUX,
    TROIS,
    QUATRE,
    CINQ,
    SIX,
    SEPT,
    HUIT,
    NEUF,
    PLUS_DEUX,
    PLUS_QUATRE,
    PASSE_TOUR,
    CHANGEMENT_SENS,
    CHANGEMENT_COULEUR
} Valeur;

typedef struct 
{
    enum Couleur Couleur;
    enum Valeur Valeur;
} Carte;

typedef struct {
    Carte* main;
    int tailleMain;
    int idSocket;
} Joueur;

