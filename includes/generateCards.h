/**
 * @file generateCards.h
 */

#ifndef CARTES_H
#define CARTES_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "struct.h"

/**
 * Génère toutes les cartes possibles dans un tableau alloué dynamiquement.
 * @return Un pointeur vers le tableau alloué contenant toutes les cartes.
 */
Carte *genererCartes();

/**
 * Melange les cartes
 * @param cartes l'ensemble des cartes à mélanger
 * @param nbCartes taille de l'ensemble des cartes
 */
void melangerCartes(Carte *cartes, int nbCartes);

/**
 * Fonction pour piocher une carte et l'ajouter à la main d'un joueur.
 * @param partie La partie contenant la pioche et les joueurs.
 * @param indexJoueur L'index du joueur qui pioche.
 */
void piocherCarte(Partie *partie, int idJoueur);

/**
 * Vérifie si une carte est "vide"
 * utile pour le début de partie 
 * @param c carte à vérifier
 * 
 * @return 1 si vide sinon 0
 */
int isCarteVide(Carte c);


#endif // CARTES_H

