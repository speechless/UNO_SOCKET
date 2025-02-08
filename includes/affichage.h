/**
 * @file affichage.h
 */

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdlib.h>
#include <stdio.h>
#include "struct.h"
#include "enhanceTerminal.h"
#include "game.h"

/**
 * Affiche une carte donnée.
 * @param c La carte à afficher.
 */
void afficherCarte(Carte c);

/**
 * Affiche une carte dans l'affichage de la main du joueur actif.
 * @param c La carte à afficher.
 * @param index index de la carte dans la main du joueur
 */
void afficherCarteMain(Carte c, int index);

/**
 * Fonction pour afficher la main du joueur avec un choix
 * @param joueur joueur actif
 * @param carteVisible la carte actuellement au dessus de la pile de jeu de la partie
 */
void afficherMainAvecSelection(Joueur joueur, Carte carteVisible);

/**
 * Affiche la main d'un joueur non actif
 * @param j joueur
 */
void afficherMain(Joueur j);

#endif // AFFICHAGE_H