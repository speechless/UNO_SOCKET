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
#include <liste.h>


void init();
void dialogueClt(client_t* client);
void traiterSignal(int sig);
void bye(void);
salon_t* getSalonPublic();
salon_t* creerSalonPublic(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost);
void gererDemandeSalon(creation_partie_t demande, client_t client);
client_t getClient(int idClient);

socket_t se;
T_Maille* listeClients = NULL;
int nextClientId = 1;
pthread_t TIDClient;

#define NB_SALONS_MAX 20
salon_t salons[NB_SALONS_MAX];
int nextSalonId = 1;

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
		client_t client;
		client.id = nextClientId++;
		client.socket = accepterClt(se);
		strcpy(client.adresse, inet_ntoa(client.socket.adrDist.sin_addr));
		client.port = ntohs(client.socket.adrDist.sin_port);

		T_Maille* mailleInseree;
		listeClients = insererEnFin(&client, listeClients, &mailleInseree);

		basic_data_t requete;
		requete.code = CLIENT;
		serialiserClient(&client, requete.data);
		envoyer(client.socket, &requete, (pFct)serialiserData);

		pthread_create(&TIDClient, NULL, (void*)dialogueClt, &mailleInseree->elt);
		pthread_detach(TIDClient);
	}

	return 0;
}

void init() {
}

void bye() {
	printf("Fermeture socket écoute\n");
	close(se.fd);

	printf("Fermeture des sockets de dialogue restantes\n");
	T_Maille* mailleCourante = listeClients;
	while (mailleCourante != NULL) {
		CHECK(close(mailleCourante->elt.socket.fd), "close client");
		mailleCourante = mailleCourante->suivant;
	}
}

void dialogueClt(client_t* client) {
	basic_data_t requete;

	bloquerSignaux();

	recevoir(client->socket, &requete, (pFct)deserialiserData);
	printf("requete recu de [%d]\n", client->socket.fd);

	switch (requete.code) {
		case CREATION_PARTIE:
			creation_partie_t demande;
			deserialiserCreationPartie(requete.data, &demande);
			gererDemandeSalon(demande, *client);
			break;
	}

	//PAUSE("Envoyer un message au client");
	//envoyer(*socket, &reponse, (pFct)serialiserData);
	afficherElt(client, "\n");
	printf("close ce client\n");
	// Fermeture de la socket de dialogue
	fprintf(stderr, "[%d]Fermeture discussion client\n", ntohs(client->socket.adrDist.sin_port));
	CHECK(close(client->socket.fd), "close()");
	client->socket.fd = -1;

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
	for (int i = 0; i < NB_SALONS_MAX; i++) {
		printf("========================\n");
		afficherSalon(salons[i]);
		if (!salons[i].isPrivate && salons[i].nbJoueursActuels < salons[i].nbJoueursMax && salons[i].id > 0) {
			fprintf(stderr, "Salon public trouvé\n");
			return &salons[i];
		}
	}
	fprintf(stderr, "Aucun salon public trouvé\n");
	return NULL;
}

salon_t* creerSalonPublic(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost) {

	salon_t* nouveauSalon = NULL;
	for (int i = 0; i < NB_SALONS_MAX; i++) {
		if (salons[i].id <= 0) {
			nouveauSalon = &salons[i];
		}
	}
	if (nouveauSalon == NULL) {
		//TODO message erreur client
		printf("Plus de place\n");
	}
	nouveauSalon->id = nextClientId++;
	nouveauSalon->isPrivate = 0;
	nouveauSalon->idHost = idHost;
	strcpy(nouveauSalon->adresseHost, adresseHost);
	printf("########%s###########\n", nouveauSalon->adresseHost);
	nouveauSalon->portHost = portHost;
	nouveauSalon->nbJoueursActuels = 0;
	nouveauSalon->nbJoueursMax = nbJoueursMax;

	return nouveauSalon;
}

void gererDemandeSalon(creation_partie_t demande, client_t client) {
	if (!demande.isPrivate) {
		salon_t* salon = getSalonPublic();

		// S'il n'y a pas de salon public on en crée un
		if (salon == NULL) {
			printf("Création d'un salon\n");
			salon = creerSalonPublic(demande.nbJoueursMax, demande.adresseHost, demande.portHost, client.id);
		}

		salon->idClients[salon->nbJoueursActuels] = client.id;
		salon->nbJoueursActuels++;
		envoyerSalon(client.socket, *salon);
	}
}

client_t getClient(int idClient) {
	T_Maille* mailleCourante = listeClients;

	while (mailleCourante != NULL) {
		if (mailleCourante->elt.id == idClient) {
			return mailleCourante->elt;
		}

		mailleCourante = mailleCourante->suivant;
	}

	client_t clientVide;
	clientVide.id = -1;

	return clientVide;
}