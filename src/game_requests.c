/**
 * @file game_requests.c
 */

#include "game_requests.h"
#include <requetes.h>
#include <common.h>

/**
 * L'host envoie aux autres jouers la partie
 * @param clients tous les joueurs de la partie
 * @param partie la partie
 */
void reqEnvoiPartie(client_t* clients, Partie* partie) {
	for (int i = 0; i < partie->nbJoueurs; i++) {
		if (clients[i].id != partie->idHost) {
			debugprintf("envoi à %d port %d\n", clients[i].id, clients[i].port);
			envoyer(clients[i].socket, partie, (pFct)serialiserPartie);
		}
	}
}

/**
 * Un joueur non host envoie la partie actualisée à l'host
 * @param socket socket avec l'host
 * @param partie la partie
 */
void reqEnvoiPartieClient(socket_t socket, Partie* partie) {
	envoyer(socket, partie, (pFct)serialiserPartie);
	debugprintf("partie envoyée à port %d\n", ntohs(socket.adrDist.sin_port));
}


/**
 * Reçoit d'un joueur la partie actualisée
 * @param socket socket avec le joueur
 * @param partie la partie
 */
void resEnvoiPartie(socket_t socket, Partie* partie) {
	debugprintf("attente de la partie du port %d (ici port %d)\n", ntohs(socket.adrDist.sin_port), ntohs(socket.adrLoc.sin_port));
	recevoir(socket, partie, (pFct)deserialiserPartie);
	debugprintf("partie reçue\n");
}

/**
 * Reçoit d'un joueur la partie actualisée après un coup joué
 * @param socket socket avec le joueur
 * @param partie la partie
 */
void resEnvoiCoup(socket_t socket, Partie* partie) {
	recevoir(socket, partie, (pFct)deserialiserCoup);
}