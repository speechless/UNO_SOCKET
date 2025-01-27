#include <serveurUNO.h>

#include <pthread.h>

#include <session.h>
#include <data.h>
#include <libPSY.h>

#include <common.h>
#include <inc.h>

#include <requetes.h>


void dialogueClt(socket_t* socket);
void traiterSignal(int sig);
void bye(void);
int getClientPort(int s);

socket_t se, sd;
pthread_t client;

int main() {
	installSignal(SIGINT, traiterSignal);
	atexit(bye);

	se = creerSocketEcoute(ADRESSE_SVC, PORT_SVC);

	int optval = 1;
	CHECK(setsockopt(se.fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt");

	// Boucle permanente de service
	while (1) {
		sd = accepterClt(se);

		pthread_create(&client, NULL, (void*)dialogueClt, &sd);
		pthread_detach(client);
	}

	return 0;
}

void bye() {
	printf("Fermeture socket écoute\n");
	close(se.fd);
}

void dialogueClt(socket_t* socket) {
	basic_data_t requete;
	basic_data_t reponse = {100, "salut"};

	bloquerSignaux();

	PAUSE("Lire un message du client");
	recevoir(*socket, &requete, (pFct)deserialiserData);

	PAUSE("Envoyer un message au client");
	envoyer(*socket, &reponse, (pFct)serialiserData);

	// Fermeture de la socket de dialogue
	fprintf(stderr, "[%d]Fermeture discussion client\n", getClientPort((*socket).fd));
	CHECK(close((*socket).fd), "close()");

	pthread_exit(NULL);
}

void traiterSignal(int sigNum) {
	switch (sigNum) {
		case SIGINT:
			printf("zdezdezze\n");
			exit(0); // Sortie par ^C
			break;
	}
}

int getClientPort(int s) {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	// Récupérer les informations de la socket (adresse et port)
	CHECK(getpeername(s, (struct sockaddr*)&addr, &addr_len), "getpeername()");

	return ntohs(addr.sin_port);
}

