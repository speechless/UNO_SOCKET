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

#include <requetes.h>


void init();
void dialogueClt(socket_t* socket);
void traiterSignal(int sig);
void bye(void);
salon_t* getSalonPublic();
salon_t* creerSalonPublic(int nbJoueursMax);
void gererDemandeSalon(demande_salon demande, socket_t socket);
socket_t* trouverSocketLibre();

socket_t se;
#define CLIENTS_MAX 20
socket_t sd[CLIENTS_MAX];
pthread_t client;

salon_t salons[2];
int nbSalons = 0;

int main() {
	installSignal(SIGINT, traiterSignal);
	atexit(bye);
	init();

	memset(&salons, 0, sizeof(salons));

	se = creerSocketEcoute(ADRESSE_SVC, PORT_SVC);

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

void init() {
	for (int i = 0; i < CLIENTS_MAX; i++) {
		sd[i].fd = -1;
	}
}

void bye() {
	printf("Fermeture socket écoute\n");
	close(se.fd);

	printf("Fermeture des sockets de dialogue restantes\n");
	for (int i = 0; i < CLIENTS_MAX; i++) {
		if (sd[i].fd == -1) {
			close(sd[i].fd);
		}
	}
}

void dialogueClt(socket_t* socket) {
	basic_data_t requete;

	bloquerSignaux();

	recevoir(*socket, &requete, (pFct)deserialiserData);
	printf("requete recu de [%d]\n", socket->fd);

	switch (requete.code) {
		case DEMANDE_SALON:
			demande_salon demande;
			deserialiserDemandeSalon(requete.data, &demande);
			gererDemandeSalon(demande, *socket);
			break;
	}

	//PAUSE("Envoyer un message au client");
	//envoyer(*socket, &reponse, (pFct)serialiserData);

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

/**
 * Cherche un salon public disponible
 * @return Un pointeur sur un salon public existant et non plein, NULL si aucun n'est trouvé
 */
salon_t* getSalonPublic() {
	for (int i = 0; i < nbSalons; i++) {
		printf("========================\n");
		afficherSalon(salons[i]);
		if (!salons[i].isPrivate && salons[i].nbJoueursActuels < salons[i].nbJoueursMax) {
			fprintf(stderr, "Salon public trouvé\n");
			return &salons[i];
		}
	}
	fprintf(stderr, "Aucun salon public trouvé\n");
	return NULL;
}

salon_t* creerSalonPublic(int nbJoueursMax) {
	salon_t* nouveauSalon = &salons[nbSalons];
	nouveauSalon->id = nbSalons;
	nouveauSalon->isHost = 1;
	nouveauSalon->isPrivate = 0;
	strcpy(nouveauSalon->adresseHost, "Aucune");
	nouveauSalon->portHost = 0;
	nouveauSalon->nbJoueursActuels = 0;
	nouveauSalon->nbJoueursMax = nbJoueursMax;

	nbSalons++;

	return nouveauSalon;
}

void gererDemandeSalon(demande_salon demande, socket_t socket) {
	if (!demande.isPrivate) {
		salon_t* salon = getSalonPublic();

		// S'il n'y a pas de salon public on en crée un
		if (salon == NULL) {
			salon = creerSalonPublic(demande.nbJoueursMax);

			salon->nbJoueursActuels++;
			envoyerSalon(socket, *salon);

			// TODO: créer socket chez client et récup le salon
		}
		// Si le salon existait déjà
		else {
			salon->isHost = 0;

			salon->nbJoueursActuels++;
			envoyerSalon(socket, *salon);
		}

	}
}

socket_t* trouverSocketLibre() {
	for (int i = 0; i < CLIENTS_MAX; i++) {
		if (sd[i].fd == -1) {
			printf("socketlibre = %d\n", i);
			return &sd[i];
		}
	}

	return NULL;
}