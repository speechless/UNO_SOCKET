#include "game_requests.h"

/*Forcement serveur*/
void reqEnvoiPartie(socket_t* sockets, Partie* partie) {
	for (int i = 0; i < partie->nbJoueurs - 1; i++) {
		envoyer(sockets[i], &partie, (pFct)serialiserPartie);
	}
}

void envoiTest(socket_t socket, int* test) {
	envoyer(socket, test, (pFct)serialiserD);  // Envoie *test (l'entier), pas &test
}

void recevoirTest(socket_t socket, int* test) {
	recevoir(socket, test, (pFct)deserialiserD);  // Passe directement test
}

int serialiserD(int* data, char* chaine) {
	sprintf(chaine, "%d", *data);
	printf("envoi : #%s#\n", chaine);
	return 0;
}

int deserialiserD(char* chaine, int* data) {
	printf("recu : #%s#\n", chaine);
	sscanf(chaine, "%d", data);  // Convertit la chaîne en int
	//*data = atoi(chaine);  // Convertit la chaîne en int
	printf("recu : %d\n", *(int*)data);
	return 0;  // Retourne 0 pour indiquer le succès
}


/*Forcement client*/
void resEnvoiPartie(socket_t socket, Partie* partie) {
	recevoir(socket, &partie, (pFct)deserialiserPartie);
}

void reqEnvoiCoupServeur(socket_t* sockets, Partie partie) {
	for (int i = 0; i < partie.nbJoueurs - 1; i++) {
		envoyer(sockets[i], &partie, (pFct)serialiserCoup);
	}
}

void reqEnvoiCoupClient(socket_t socketHost, Partie partie) {
	envoyer(socketHost, &partie, (pFct)serialiserCoup);
}

void resEnvoiCoup(socket_t socket, Partie partie) {
	recevoir(socket, &partie, (pFct)deserialiserCoup);
}