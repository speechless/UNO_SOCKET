/**
 * @file game.h
 */

#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "struct.h"
#include "enhanceTerminal.h"
#include "generateCards.h"
#include "affichage.h"
#include "serialize.h"
#include "game_requests.h"
#include <requetes.h>

/**
 * initalise une partie
 * @param partie la partie à initialiser
 * @param nbJoueurs le nombre de joueur de la partie
 * @param clients les joueurs
 * @param idHost l'host de la partie
 */
void initPartie(Partie* partie, int nbJoueurs, client_t* clients, int idHost);

/**
 * Fonction pour jouer une carte
 * @param partie partie actuelle
 * @param idJoueur le joueur qui joue la carte
 * @param carteJouee la carte qu'il souhaite jouer
 * 
 * @return renvoie 1 si l'action s'est faite et 0 si erreur
 */ 
int jouerCarte(Partie* partie, int idJoueur, Carte carteJouee);

void afficherMainAvecSelection(Joueur joueur, Carte carteVisible);

/**
 * Fait passer la partie au tour suivant
 * @param partie La partie actuelle
 */
void prochainTour(Partie* partie);

/**
 * Donne l'index d'un client en fonction de son id
 * @param id id du joueur
 * @param joueurs tous les joueurs de la partie
 * @param nbJoueurs nombre de joueur de la partie
 * 
 * @return -1 si pas trouvé sinon son ID
 */
int getIndexFromIdClient(int id, client_t* joueurs, int nbJoueurs);

/**
 * Donne l'index dans la partie d'un joueur en fonction de son id client
 * @param id id du joueur
 * @param joueurs tous les joueurs de la partie
 * @param nbJoueurs nombre de joueur de la partie
 * 
 * @return -1 si pas trouvé sinon son ID
 */
int getIndexFromIdJoueur(int id, Joueur* joueurs, int nbJoueurs);

#endif // GAME_H