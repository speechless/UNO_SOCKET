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


void gererConnexion();
void dialogueClt(client_t* client);
void traiterSignal(int sig);
void bye(void);
salon_t* getSalonPublic();
salon_t* creerSalonPublic(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost);
salon_t* creerSalonPrive(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost, char* code);
void gererCreationPartie(creation_partie_t demande, client_t client);
void gererRejoindrePartie(rejoindre_partie_t demande);
void ajouterJoueurSalon(salon_t* salon, int idClient);
void retirerJoueurSalon(salon_t* salon, int idClient);
client_t getClient(int idClient);
salon_t* getSalonClient(int idClient);

socket_t se;
T_Maille* listeClients = NULL;
int nextClientId = 1;
pthread_t TIDClient;

#define NB_SALONS_MAX 10
salon_t salons[NB_SALONS_MAX];
int nextSalonId = 1;

int main() {
	installSignal(SIGINT, traiterSignal);
	atexit(bye);

	memset(&salons, 0, sizeof(salons));

	se = creerSocketEcoute(ADRESSE_SVC, PORT_SVC);

	int optval = 1;
	CHECK(setsockopt(se.fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)), "setsockopt");

	// Boucle permanente de service
	while (1) {
		gererConnexion();
	}

	return 0;
}

void gererConnexion() {
	client_t client;

	// Récupération des informations du client
	client.id = nextClientId++;
	client.socket = accepterClt(se);
	strcpy(client.adresse, inet_ntoa(client.socket.adrDist.sin_addr));
	client.port = ntohs(client.socket.adrDist.sin_port);

	fprintf(stderr, "Connexion de %s:%d (ID = %d)\n", client.adresse, client.port, client.id);

	// Insertion dans la liste des clients connectés
	T_Maille* mailleInseree;
	listeClients = insererEnFin(&client, listeClients, &mailleInseree);

	// Envoi de ses informations au client
	basic_data_t requete;
	requete.code = CLIENT;
	serialiserClient(&client, requete.data);
	envoyer(client.socket, &requete, (pFct)serialiserData);

	// Création d'un thread pour communiquer
	pthread_create(&TIDClient, NULL, (void*)dialogueClt, &mailleInseree->elt);
	pthread_detach(TIDClient);
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
	salon_t* salonClient;

	bloquerSignaux();

	do {

		printf("==========================\nid salons : ");
		for (int i = 0; i < NB_SALONS_MAX; i++) {
			printf("%d, ", salons[i].id);
		}
		printf("\n=================\n");

		// Réception d'une requête
		recevoir(client->socket, &requete, (pFct)deserialiserData);
		fprintf(stderr, "Requête reçue du client n°%d\n", client->id);

		switch (requete.code) {
			case DECONNEXION:
				fprintf(stderr, "Requête de type DECONNEXION\n");

				salonClient = getSalonClient(client->id);
				if (salonClient != NULL) {
					retirerJoueurSalon(salonClient, client->id);
				}
				break;

			case QUITTER_PARTIE:
				fprintf(stderr, "Requête de type QUITTER_PARTIE\n");

				salonClient = getSalonClient(client->id);
				if (salonClient != NULL) {
					retirerJoueurSalon(salonClient, client->id);
				}
				break;

			case CREATION_PARTIE:
				creation_partie_t demandeCreation;

				fprintf(stderr, "Requête de type CREATION_PARTIE\n");

				deserialiserCreationPartie(requete.data, &demandeCreation);
				gererCreationPartie(demandeCreation, *client);
				break;

			case REJOINDRE_PARTIE:
				rejoindre_partie_t demandeRejoindre;

				fprintf(stderr, "Requête de type REJOINDRE_PARTIE\n");

				deserialiserRejoindrePartie(requete.data, &demandeRejoindre);
				gererRejoindrePartie(demandeRejoindre);
				break;
		}

	} while (requete.code != DECONNEXION);

	// Fermeture de la socket de dialogue
	fprintf(stderr, "Fermeture de la discussion avec le client n°%d\n", client->id);
	CHECK(close(client->socket.fd), "close()");
	listeClients = supprimerElement(*client, listeClients);

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
		if (!salons[i].isPrivate && salons[i].nbJoueursActuels < salons[i].nbJoueursMax && salons[i].id > 0) {
			fprintf(stderr, "Salon public trouvé\n");
			return &salons[i];
		}
	}
	fprintf(stderr, "Aucun salon public trouvé\n");
	return NULL;
}

/**
 * Cherche un salon privé à partir d'un code
 * @param code Le code associé au salon
 * @return Un pointeur sur un salon privé avec le code correspondant et non plein, NULL si aucun n'est trouvé
 */
salon_t* getSalonPrive(char* code) {
	for (int i = 0; i < NB_SALONS_MAX; i++) {
		if (salons[i].id > 0 && salons[i].isPrivate && strcmp(salons[i].code, code) == 0) {

			if (salons[i].nbJoueursActuels < salons[i].nbJoueursMax) {
				fprintf(stderr, "Salon privé correspondant trouvé\n");
				return &salons[i];
			}
			else {
				fprintf(stderr, "Salon correspondant trouvé mais plein\n");
				return NULL;
			}
		}
	}
	fprintf(stderr, "Aucun salon n'existe avec ce code\n");
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
		fprintf(stderr, "Plus de place\n");
		return NULL;
	}

	nouveauSalon->id = nextSalonId++;
	nouveauSalon->isPrivate = 0;
	nouveauSalon->idHost = idHost;
	strcpy(nouveauSalon->adresseHost, adresseHost);
	nouveauSalon->portHost = portHost;
	nouveauSalon->nbJoueursActuels = 0;
	nouveauSalon->nbJoueursMax = nbJoueursMax;

	return nouveauSalon;
}

salon_t* creerSalonPrive(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost, char* code) {
	salon_t* nouveauSalon = NULL;

	for (int i = 0; i < NB_SALONS_MAX; i++) {
		if (salons[i].id <= 0) {
			nouveauSalon = &salons[i];
		}
	}
	if (nouveauSalon == NULL) {
		fprintf(stderr, "Plus de place\n");
		return NULL;
	}

	nouveauSalon->id = nextSalonId++;
	nouveauSalon->isPrivate = 1;
	nouveauSalon->idHost = idHost;
	strcpy(nouveauSalon->adresseHost, adresseHost);
	nouveauSalon->portHost = portHost;
	nouveauSalon->nbJoueursActuels = 0;
	nouveauSalon->nbJoueursMax = nbJoueursMax;
	strcpy(nouveauSalon->code, code);

	return nouveauSalon;
}

//TODO : générer le code
void gererCreationPartie(creation_partie_t demande, client_t client) {
	salon_t* salon;

	if (!demande.isPrivate) {
		printf("Création d'un salon public\n");
		salon = creerSalonPublic(demande.nbJoueursMax, demande.adresseHost, demande.portHost, client.id);

		if (salon == NULL) {
			envoyerErreur(client.socket, "Trop de salons en attente, veuillez réessayer plus tard");
			return;
		}
	}
	else {
		printf("Création d'un salon privé\n");
		salon = creerSalonPrive(demande.nbJoueursMax, demande.adresseHost, demande.portHost, client.id, "1234");

		if (salon == NULL) {
			envoyerErreur(client.socket, "Trop de salons en attente, veuillez réessayer plus tard");
			return;
		}
	}

	ajouterJoueurSalon(salon, client.id);
}

void gererRejoindrePartie(rejoindre_partie_t demande) {
	client_t client = getClient(demande.idClient);

	// Si le client souhaite rejoindre une partie publique
	if (!demande.isPrivate) {
		salon_t* salon = getSalonPublic();

		// S'il n'y a pas de salon public on demande au client de créer une partie
		if (salon == NULL) {
			basic_data_t requete;
			requete.code = CREATION_PARTIE;
			requete.data[0] = '\0';
			envoyer(client.socket, &requete, (pFct)serialiserData);
		}
		else {
			ajouterJoueurSalon(salon, client.id);
		}

	}
	// Si le client souhaite rejoindre une partie privée
	else {
		salon_t* salon = getSalonPrive(demande.code);

		if (salon != NULL) {
			ajouterJoueurSalon(salon, client.id);
		}
		else {
			envoyerErreur(client.socket, "Code incorrect ou salon plein");
		}
	}
}

void ajouterJoueurSalon(salon_t* salon, int idClient) {
	// Ajout du client aux joueurs
	salon->idClients[salon->nbJoueursActuels] = idClient;
	salon->nbJoueursActuels++;

	// Envoi de l'état actualisé du salon à tout le monde
	for (int i = 0; i < salon->nbJoueursActuels; i++) {
		client_t joueurDuSalon = getClient(salon->idClients[i]);
		envoyerSalon(joueurDuSalon.socket, *salon);
	}

	// Démarrage de la partie si complet
	if (salon->nbJoueursActuels == salon->nbJoueursMax) {
		fprintf(stderr, "Envoi du signal de démarrage\n");

		basic_data_t requete;
		requete.code = COMMENCER_PARTIE;
		requete.data[0] = '\0';

		for (int i = 0; i < salon->nbJoueursActuels; i++) {
			client_t joueurDuSalon = getClient(salon->idClients[i]);
			envoyer(joueurDuSalon.socket, &requete, (pFct)serialiserData);
		}

		// Réinitialisation du salon
		memset(salon, 0, sizeof(salon_t));
	}
}


void retirerJoueurSalon(salon_t* salon, int idClient) {
	// Suppression du client dans joueurs présents
	int i = 0;
	while (i < salon->nbJoueursActuels && salon->idClients[i] != idClient) {
		i++;
	}
	// i est à l'index du joueur à retirer
	i += 1; // i est à droite du joueurs à retirer
	while (i < salon->nbJoueursActuels) {
		salon->idClients[i - 1] = salon->idClients[i];
	}

	salon->nbJoueursActuels--;

	if (salon->nbJoueursActuels > 0) {
		// Envoi de l'état actualisé du salon à tout le monde
		for (int i = 0; i < salon->nbJoueursActuels; i++) {
			client_t joueurDuSalon = getClient(salon->idClients[i]);
			envoyerSalon(joueurDuSalon.socket, *salon);
		}
	}
	else {
		// Réinitialisation du salon
		memset(salon, 0, sizeof(salon_t));
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

salon_t* getSalonClient(int idClient) {
	salon_t* salonCourant;

	for (int i = 0; i < NB_SALONS_MAX; i++) {
		salonCourant = &salons[i];

		if (salonCourant->id > 0) {
			for (int j = 0; j < salonCourant->nbJoueursActuels; j++) {
				if (salonCourant->idClients[j] == idClient) {
					return salonCourant;
				}
			}
		}
	}

	return NULL;
}