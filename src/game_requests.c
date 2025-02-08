#include "game_requests.h"
#include <requetes.h>
#include <common.h>

/*Forcement serveur*/
void reqEnvoiPartie(client_t* clients, Partie* partie) {
	for (int i = 0; i < partie->nbJoueurs; i++) {
		if (clients[i].id != partie->idHost) {
			debugprintf("envoi à %d port %d\n", clients[i].id, clients[i].port);
			envoyer(clients[i].socket, partie, (pFct)serialiserPartie);
		}
	}
}

void reqEnvoiPartieClient(socket_t socket, Partie* partie) {
	envoyer(socket, partie, (pFct)serialiserPartie);
}

void envoiTest(socket_t socket, int* test) {
	envoyer(socket, test, (pFct)serialiserD);  // Envoie *test (l'entier), pas &test
}

void recevoirTest(socket_t socket, int* test) {
	recevoir(socket, test, (pFct)deserialiserD);  // Passe directement test
}

int serialiserD(int* data, char* chaine) {
	sprintf(chaine, "%d", *data);
	debugprintf("envoi : #%s#\n", chaine);
	return 0;
}

int deserialiserD(char* chaine, int* data) {
	debugprintf("recu : #%s#\n", chaine);
	sscanf(chaine, "%d", data);  // Convertit la chaîne en int
	//*data = atoi(chaine);  // Convertit la chaîne en int
	debugprintf("recu : %d\n", *(int*)data);
	return 0;  // Retourne 0 pour indiquer le succès
}


/*Forcement client*/
void resEnvoiPartie(socket_t socket, Partie* partie) {
	recevoir(socket, partie, (pFct)deserialiserPartie);
}
/*
void reqEnvoiCoupServeur(socket_t* sockets, Partie* partie) {
	for (int i = 1; i < partie->nbJoueurs - 1; i++) {
		envoyer(sockets[i], partie, (pFct)serialiserCoup);
	}


void reqEnvoiCoupClient(socket_t socketHost, Partie* partie) {
	envoyer(socketHost, partie, (pFct)serialiserCoup);
}
*/
void resEnvoiCoup(socket_t socket, Partie* partie) {
	recevoir(socket, partie, (pFct)deserialiserCoup);
}