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


void dialogueClt(socket_t* socket);
void traiterSignal(int sig);
void bye(void);
salon_t* getSalonPublic();
salon_t* creerSalonPublic(int nbJoueursMax);

socket_t se, sd;
pthread_t client;

salon_t salons[20];
int nbSalons = 0;

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

	bloquerSignaux();

	PAUSE("Lire un message du client");
	recevoir(*socket, &requete, (pFct)deserialiserData);

	if (requete.code == DEMANDE_SALON) {
		demande_salon demande;
		PAUSE("désérialiser demande");
		deserialiserDemandeSalon(requete.data, &demande);

		if (!demande.isPrivate) {
			salon_t* salon = getSalonPublic();

			// S'il n'y a pas de salon public on en crée un
			if (salon == NULL) {
				salon = creerSalonPublic(demande.nbJoueursMax);

				salon->nbJoueursActuels++;
				envoyerSalon(*socket, *salon);

				// TODO: créer socket chez client et récup le salon
			}
			// Si le salon existait déjà
			else {
				salon->isHost = 0;

				salon->nbJoueursActuels++;
				envoyerSalon(*socket, *salon);
			}

		}


	}

	//PAUSE("Envoyer un message au client");
	//envoyer(*socket, &reponse, (pFct)serialiserData);

	// Fermeture de la socket de dialogue
	fprintf(stderr, "[%d]Fermeture discussion client\n", ntohs((*socket).adrDist.sin_port));
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

/**
 * Cherche un salon public disponible
 * @return Un pointeur sur un salon public existant et non plein, NULL si aucun n'est trouvé
 */
salon_t* getSalonPublic() {
	for (int i = 0; i < nbSalons; i++) {
		if (!salons[i].isPrivate && salons[i].nbJoueursActuels < salons[i].nbJoueursMax) {
			return &salons[i];
		}
	}

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

	return nouveauSalon;
}