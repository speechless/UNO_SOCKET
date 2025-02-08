/**
 * @file player.h
 */

#include "struct.h"
#include "enhanceTerminal.h"
#include "affichage.h"
#include "serialize.h"
#include "game_requests.h"
#include "game.h"
#include "generateCards.h"

/**
 * Déroulement de la partie côté host
 * @param partie la partie actuelle
 * @param idJoueur id du joueur
 * @param socketHost tous les clients qui deviennent les joueurs de la partie
 */
int jouerPartieServeur(Partie* partie, int idJoueur, client_t* sockets);

/**
 * Déroulement de la partie côté client
 * @param partie la partie actuelle
 * @param idJoueur id du joueur
 * @param socketHost socket avec l'host de la partie
 */
int jouerPartieClient(Partie* partie, int idJoueur, socket_t socketHost);