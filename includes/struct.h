/**
 * @file struct.h
 */

#ifndef STRUCT_H
#define STRUCT_H

#include "session.h"

#define NB_COULEURS 4 /**< Nombre de couleurs ROUGE, BLEU, JAUNE, VERT (sans NOIR)*/
#define NB_VALEURS 13 /**< Valeurs de ZERO à CHANGEMENT_SENS (sans PLUS_QUATRE et CHANGEMENT_COULEUR)*/
#define NB_SPECIALES 1 /**< CHANGEMENT_COULEUR */

#define TAILLE_MAIN 5 /**< Taille main de base d'un joueur */
#define TAILLE_MAIN_MAX 150 /**< Taille maximale main d'un joueur */

#define TOTAL_CARTES ((NB_COULEURS * NB_VALEURS * 2) + (NB_SPECIALES * 4))  /**<  Cartes normales * 2 + Cartes spéciales*/

/**
 * Couleurs possibles des cartes
 */
typedef enum {
	ROUGE,
	BLEU,
	JAUNE,
	VERT,
	NOIR
} Couleur;

/**
 * Valeurs possibles des cartes
 */
typedef enum {
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
	CHANGEMENT_COULEUR
} Valeur;

/**
 * Carte du jeu
 */
typedef struct {
	Couleur Couleur;
	Valeur Valeur;
} Carte;

/**
 * Joueur 
 */
typedef struct {
	socket_t idSocket;
	int idJoueur;

	int tailleMain;
	Carte* main;
} Joueur;

/**
 * Partie
 */
typedef struct
{
	int nbJoueurs;
	int currentPlayer;
	int sens;               // 1 ou -1
	Joueur joueurs[4];      //index 0 est l'hébergeur

	Carte carteVisible;
	int nbCartesPioche;
	Carte* pioche;

	int estFinie;
	int idHost;
} Partie;

#endif // STRUCT_H

