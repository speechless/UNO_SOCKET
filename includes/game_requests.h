/**
 * @file game_requests.h
 */

#ifndef REQ_GAME_H
#define REQ_GAME_H

#include "serialize.h"
#include "struct.h"
#include <stdlib.h>
#include <data.h>
#include <requetes.h>

/**
 * L'host envoie aux autres jouers la partie
 * @param clients tous les joueurs de la partie
 * @param partie la partie
 */
void reqEnvoiPartie(client_t* sockets, Partie* partie);

/**
 * Reçoit d'un joueur la partie actualisée
 * @param socket socket avec le joueur
 * @param partie la partie
 */
void resEnvoiPartie(socket_t socket, Partie* partie);

/**
 * Un joueur non host envoie la partie actualisée à l'host
 * @param socket socket avec l'host
 * @param partie la partie
 */
void reqEnvoiPartieClient(socket_t socket, Partie* partie);

/**
 * Reçoit d'un joueur la partie actualisée après un coup joué
 * @param socket socket avec le joueur
 * @param partie la partie
 */
void resEnvoiCoup(socket_t socket, Partie* partie);

#endif // REQ_GAME_H