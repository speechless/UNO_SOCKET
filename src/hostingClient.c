#include <requetes.h>
#include <pthread.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <session.h>
#include <data.h>
#include <libPSY.h>

#include <common.h>
#include <inc.h>
#include <stdio.h>

int main()
{
    salon_t salon;
    socket_t se;
    initSalon(&salon,0,0,0,"127.0.0.1",5000,1,3);
    se = creerSocketEcoute("127.0.0.1", 5000);
    afficherSalon(salon);

    int optval = 1;
	CHECK(setsockopt(se.fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt");

	// Boucle permanente de service
	while (1) {
		socket_t* socketLibre;
		do {
			socketLibre = trouverSocketLibre();
		} while (socketLibre == NULL);

		*socketLibre = accepterClt(se);

		pthread_create(&client, NULL, (void*)dialogueClt, socketLibre);
		pthread_detach(client);
	}


    return 0;
}

void dialogueClt(socket_t* socket) {
	basic_data_t requete;

	bloquerSignaux();

	recevoir(*socket, &requete, (pFct)deserialiserData);
	printf("requete recu de [%d]\n", socket->fd);

	switch (requete.code) {
        case DEMANDE_CONNECTION:
            printf("Demande de connection\n");
            break;
            
		case DEMANDE_SALON:
			demande_salon demande;
			deserialiserDemandeSalon(requete.data, &demande);
			gererDemandeSalon(demande, *socket);
			break;
	}

	// Fermeture de la socket de dialogue
	fprintf(stderr, "[%d]Fermeture discussion client\n", ntohs((*socket).adrDist.sin_port));
	CHECK(close(socket->fd), "close()");
	socket->fd = -1;

	pthread_exit(NULL);
}

void traiterSignal(int sigNum) {
	switch (sigNum) {
		case SIGINT:
			exit(0); // Sortie par ^C
			break;
	}
}

