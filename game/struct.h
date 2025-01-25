#ifndef STRUCT_H
#define STRUCT_H

#define NB_COULEURS 4 // ROUGE, BLEU, JAUNE, VERT (sans NOIR)
#define NB_VALEURS 13 // Valeurs de ZERO à CHANGEMENT_SENS (sans PLUS_QUATRE et CHANGEMENT_COULEUR)
#define NB_SPECIALES 2 // PLUS_QUATRE, CHANGEMENT_COULEUR

#define TAILLE_MAIN 5
#define TAILLE_MAIN_MAX 108

#define TOTAL_CARTES ((NB_COULEURS * NB_VALEURS * 2) + (NB_SPECIALES * 4)) // Cartes normales * 2 + Cartes spéciales

typedef enum{
    ROUGE,
    BLEU,
    JAUNE,
    VERT,
    NOIR
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
    PASSE_TOUR,
    CHANGEMENT_SENS,
    PLUS_QUATRE,
    CHANGEMENT_COULEUR
} Valeur;

typedef struct {
    Couleur Couleur;
    Valeur Valeur;
} Carte;

typedef struct {
    Carte* main;
    int tailleMain;
    int idSocket;
    int idJoueur;
} Joueur;

typedef struct
{
    Carte* pioche;
    int nbCartesPioche;
    Joueur joueurs[4];      //index 0 est l'hébergeur
    int nbJoueurs;
    int currentPlayer;
    int sens;               // 1 ou -1
    int nbTours;
    Carte carteVisible;
    int estFinie;
} Partie;

#endif // STRUCT_H

