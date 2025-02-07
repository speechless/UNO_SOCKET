/**
 * @file serveurUNO.c
 */

 /**
  * TODO:
  * mutex
  */


#include <pthread.h>
#include <string.h>
#include <time.h>

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
salon_t* creerSalonPrive(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost, int code);
void gererCreationPartie(creation_partie_t demande, client_t client);
void gererRejoindrePartie(rejoindre_partie_t demande);
void ajouterJoueurSalon(salon_t* salon, int idClient);
void retirerJoueurSalon(salon_t* salon, int idClient);
client_t getClient(int idClient);
void supprimerSalon(salon_t* salon);
salon_t* getSalonClient(int idClient);
int generateCode();

socket_t se; /**< Socket d'écoute du serveur */

T_Maille* listeClients = NULL; /**< Liste chainee des clients connectés */
int nextClientId = 1; /**< ID pour le prochain client */
pthread_mutex_t mutexListeClients = PTHREAD_MUTEX_INITIALIZER; /**< Mutex pour synchroniser l'accès à la liste des clients */

pthread_t TIDClient; /**< ID du thread pour gérer les connexions des clients */

#define NB_SALONS_MAX 10 /**< Nombre maximum de salons */
salon_t salons[NB_SALONS_MAX]; /**< Tableau de salons */
int nextSalonId = 1; /**< ID pour le prochain salon */
pthread_mutex_t mutexListeSalons = PTHREAD_MUTEX_INITIALIZER; /**< Mutex pour synchroniser l'accès à la liste des salons */

int codes[100] = {-1}; /**< Codes des salons privés (-1 si non attribué) */
pthread_mutex_t mutexListeCodes = PTHREAD_MUTEX_INITIALIZER; /**< Mutex pour synchroniser l'accès à la liste des codes */


/**
 * Fonction principale qui permet la mise en relation des clients
 */
int main() {
	installSignal(SIGINT, traiterSignal);
	atexit(bye);

	srand(time(NULL));

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

/**
 * S'occupe d'accepter une connexion entrante d'un client, de lui envoyer son id et de créer un thread de service
 */
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
	CHECK(pthread_mutex_lock(&mutexListeClients), "lock liste clients");
	listeClients = insererEnFin(&client, listeClients, &mailleInseree);
	CHECK(pthread_mutex_unlock(&mutexListeClients), "unlock liste clients");


	// Envoi de ses informations au client
	basic_data_t requete;
	requete.code = CLIENT;
	serialiserClient(&client, requete.data);
	envoyer(client.socket, &requete, (pFct)serialiserData);

	// Création d'un thread pour communiquer
	pthread_create(&TIDClient, NULL, (void*)dialogueClt, &mailleInseree->elt);
	pthread_detach(TIDClient);
}

/**
 * Fonction appelée à la fin du programme
 * Ferme la socket d'écoute ainsi que les sockets de dialogue
 */
void bye() {
	printf("Fermeture socket écoute\n");
	CHECK(close(se.fd), "close socket écoute");

	printf("Fermeture des sockets de dialogue restantes\n");
	T_Maille* mailleCourante = listeClients;
	while (mailleCourante != NULL) {
		CHECK(close(mailleCourante->elt.socket.fd), "close client");
		mailleCourante = mailleCourante->suivant;
	}
}

/**
 * Fonction exécutée par les threads de service
 * Reçoit toutes les requêtes d'un client et s'occupe de les gérer
 * @param client Pointeur sur la case du client à servir dans la liste chainée
 */
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
		printf("==========================\nid salons : ");
		afficherListe(listeClients);
		printf("\n=================\n");

		// Réception d'une requête
		recevoir(client->socket, &requete, (pFct)deserialiserData);
		fprintf(stderr, "Requête reçue du client n°%d\n", client->id);

		switch (requete.code) {
			case DECONNEXION:
				fprintf(stderr, "Requête de type DECONNEXION\n");

				CHECK(pthread_mutex_lock(&mutexListeSalons), "lock liste salons");

				salonClient = getSalonClient(client->id);
				if (salonClient != NULL) {
					retirerJoueurSalon(salonClient, client->id);
				}

				CHECK(pthread_mutex_unlock(&mutexListeSalons), "unlock liste salons");
				break;

			case QUITTER_PARTIE:
				fprintf(stderr, "Requête de type QUITTER_PARTIE\n");

				CHECK(pthread_mutex_lock(&mutexListeSalons), "lock liste salons");

				salonClient = getSalonClient(client->id);
				if (salonClient != NULL) {
					retirerJoueurSalon(salonClient, client->id);
				}

				CHECK(pthread_mutex_unlock(&mutexListeSalons), "unlock liste salons");
				break;

			case CREATION_PARTIE:
				creation_partie_t demandeCreation;

				fprintf(stderr, "Requête de type CREATION_PARTIE\n");

				deserialiserCreationPartie(requete.data, &demandeCreation);

				CHECK(pthread_mutex_lock(&mutexListeSalons), "lock liste salons");
				gererCreationPartie(demandeCreation, *client);
				CHECK(pthread_mutex_unlock(&mutexListeSalons), "unlock liste salons");

				break;

			case REJOINDRE_PARTIE:
				rejoindre_partie_t demandeRejoindre;

				fprintf(stderr, "Requête de type REJOINDRE_PARTIE\n");

				deserialiserRejoindrePartie(requete.data, &demandeRejoindre);

				CHECK(pthread_mutex_lock(&mutexListeSalons), "lock liste salons");
				gererRejoindrePartie(demandeRejoindre);
				CHECK(pthread_mutex_unlock(&mutexListeSalons), "unlock liste salons");

				break;
		}

	} while (requete.code != DECONNEXION);


	// Fermeture de la socket de dialogue
	fprintf(stderr, "Fermeture de la discussion avec le client n°%d\n", client->id);
	CHECK(close(client->socket.fd), "close()");

	// Suppression du client de la liste

	CHECK(pthread_mutex_lock(&mutexListeClients), "lock liste clients");
	listeClients = supprimerElement(*client, listeClients);
	CHECK(pthread_mutex_unlock(&mutexListeClients), "unlock liste clients");

	pthread_exit(NULL);
}

/**
 * Fonction appelée pour traiter le signal SIGINT
 * @param sigNum Numéro du signal à traiter
 */
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
salon_t* getSalonPrive(int code) {
	for (int i = 0; i < NB_SALONS_MAX; i++) {
		if (salons[i].id > 0 && salons[i].isPrivate && salons[i].code == code) {

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

/**
 * Créer un salon de type public
 * @param nbJoueursMax Nombre maximal de joueurs dans la partie
 * @param adresseHost Adresse IP de l'hébergeur de la partie en notation pointée x.x.x.x
 * @param portHost Port de l'hébergeur de la partie
 * @param idHost Id du client hébergeur de la partie
 */
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
	nouveauSalon->code = 0;

	return nouveauSalon;
}

/**
 * Créer un salon de type privé
 * @param nbJoueursMax Nombre maximal de joueurs dans la partie
 * @param adresseHost Adresse IP de l'hébergeur de la partie en notation pointée x.x.x.x
 * @param portHost Port de l'hébergeur de la partie
 * @param idHost Id du client hébergeur de la partie
 * @param code Code qui sera à rentrer par les joueurs pour rejoindre la partie
 */
salon_t* creerSalonPrive(int nbJoueursMax, char* adresseHost, unsigned short portHost, int idHost, int code) {
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
	nouveauSalon->code = code;

	return nouveauSalon;
}

/**
 * Gère la création d'une partie privée ou publique. Envoie au client un message d'erreur si un problème est survenu
 * S'il n'y a pas de problème le client est automatiquement ajouté au salon
 * @param demande Paramètres pour la création de la partie
 * @param client Client à l'initiative de la création
 */
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
		int codeGenere = generateCode();

		// S'il n'y a plus de place pour stocker le code du salon
		if (codeGenere == -1) {
			envoyerErreur(client.socket, "Trop de salons en attente, veuillez réessayer plus tard");
			return;
		}

		salon = creerSalonPrive(demande.nbJoueursMax, demande.adresseHost, demande.portHost, client.id, codeGenere);

		if (salon == NULL) {
			envoyerErreur(client.socket, "Trop de salons en attente, veuillez réessayer plus tard");
			return;
		}
	}

	ajouterJoueurSalon(salon, client.id);
}

/**
 * Gère la demande d'un client pour rejoindre une partie publique ou privée.
 *
 * Si la partie est publique, le client est ajouté à un salon public existant ou invité à
 * créer une nouvelle partie si aucun salon n'existe.
 * Si la partie est privée, le client est ajouté au salon privé correspondant au code fourni
 * ou reçoit un message d'erreur si le code est incorrect ou si le salon est plein.
 *
 * @param demande Structure contenant les informations de la demande de rejoindre une partie.
 */
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

/**
 * @brief Ajoute un joueur à un salon et gère l'état du salon.
 *
 * Cette fonction ajoute un joueur à un salon en mettant à jour la liste des
 * joueurs et en envoyant l'état actualisé du salon à tous les joueurs présents.
 * Si le salon atteint le nombre maximum de joueurs, la partie est démarrée et
 * un signal de démarrage est envoyé à tous les joueurs. Le salon est ensuite supprimé.
 *
 * @param salon Pointeur vers la structure du salon.
 * @param idClient Identifiant du client à ajouter au salon.
 */
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

		supprimerSalon(salon);
	}
}

/**
 * @brief Retire un joueur d'un salon et gère l'état du salon.
 *
 * Cette fonction retire un joueur d'un salon en mettant à jour la liste des
 * joueurs et en envoyant l'état actualisé du salon à tous les joueurs présents.
 * Si le salon devient vide après le retrait du joueur, le salon est supprimé.
 *
 * @param salon Pointeur vers la structure du salon.
 * @param idClient Identifiant du client à retirer du salon.
 */
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
		supprimerSalon(salon);
	}

}

/**
 * Supprime un salon et rend disponible son code
 * @param salon Pointeur sur le salon à supprimer
 */
void supprimerSalon(salon_t* salon) {

	if (salon->isPrivate) {
		// Libération du code privé
		for (int i = 0; i < 100; i++) {
			if (codes[i] == salon->code) {
				codes[i] = -1;
			}
		}
	}

	// Réinitialisation du salon
	memset(salon, 0, sizeof(salon_t));
}

/**
 * Cherche les informations d'un client
 * @param idClient Id du client à chercher
 * @return Une structure client_t contenant les informations du client, un client vide si non trouvé
 */
client_t getClient(int idClient) {
	T_Maille* mailleCourante = listeClients;

	CHECK(pthread_mutex_lock(&mutexListeClients), "lock liste clients");

	while (mailleCourante != NULL) {
		if (mailleCourante->elt.id == idClient) {
			CHECK(pthread_mutex_unlock(&mutexListeClients), "unlock liste clients");
			return mailleCourante->elt;
		}

		mailleCourante = mailleCourante->suivant;
	}

	CHECK(pthread_mutex_unlock(&mutexListeClients), "unlock liste clients");


	client_t clientVide;
	clientVide.id = -1;
	clientVide.port = 0;
	clientVide.socket.fd = -1;
	clientVide.adresse[0] = '\0';

	return clientVide;
}

/**
 * Cherche le salon dans lequel se trouve un client
 * @param idClient Id du client à chercher
 * @return Un pointeur sur le salon dans lequel se trouve le client, NULL si non trouvé
 */
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

/**
 * Génère un code et le place dans le tableau des codes
 * @return Le code généré ou -1 tableau plein
 */
int generateCode() {
	int isHereFlag = 0;

	int code = rand() % 10000;

	CHECK(pthread_mutex_lock(&mutexListeCodes), "lock liste codes");


	// Vérification si code déjà existant
	for (int i = 0; i < 100; i++) {
		if (codes[i] == code) {
			isHereFlag = 1;
			break;
		}
	}

	// Si code inexistant, on trouve une place disponible
	if (!isHereFlag) {
		for (int i = 0; i < 100; i++) {
			if (codes[i] <= 0) {
				codes[i] = code;
				CHECK(pthread_mutex_unlock(&mutexListeCodes), "unlock liste codes");

				return code;
			}
		}
		CHECK(pthread_mutex_unlock(&mutexListeCodes), "unlock liste codes");
		return -1;
	}
	else {
		CHECK(pthread_mutex_unlock(&mutexListeCodes), "unlock liste codes");
		return generateCode();
	}
}