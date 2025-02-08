/**
 * @file clientUNO.c
 */

#include <session.h>
#include <data.h>
#include <enhanceTerminal.h>
#include <libPSY.h>

#include <inc.h>
#include <common.h>

#include <unistd.h>
#include <string.h>

#include <requetes.h>

#include <struct.h>
#include <game.h>
#include <player.h>
#include <game_requests.h>
#include <serialize.h>

void traiterSignal(int sigNum);
void bye();
void deconnexionServeurUNO();
client_t connexionServeurUNO();
void quitterSalon();
void afficherMenu();
void lancerPartiePublique(client_t clientLocal);
void rejoindrePartiePrivee(client_t clientLocal, int code);
void creerPartiePrivee(client_t clientLocal, int nbJoueursMax);
client_t* initConnection(salon_t salon);

socket_t socketAppel;
socket_t socketEcouteHebergeur = {-1,};
socket_t socketPartie;
int connecte = 0;

/**
 * Programme principal du client
 */
int main() {

	salon_t salon;
	client_t clientLocal;

	basic_data_t requete = {-1, ""};
	creation_partie_t demandeCreation;
	int input;
	int codePrive;
	int nbJoueursPrive;

	Partie* partie;
	client_t* clients;


	installSignal(SIGINT, traiterSignal);
	atexit(bye);


	while (1) {

		requete.code = -1;
		input = 0;

		if (!connecte) {
			// Connexion au hub de jeu
			clientLocal = connexionServeurUNO();
			connecte = 1;
		}

		while (input != 1 && input != 2 && input != 3) {
			afficherMenu();
			scanf("%d", &input);
			switch (input) {
				case 1:
					// Lancer une partie publique
					lancerPartiePublique(clientLocal);
					clearScreen();
					setTerm(BLUE);
					printf("\nEn attente de partie...\n");
					resetTerm();
					break;
				case 2:
					clearScreen();

					// Rejoindre une partie privée
					printf("Entrez le code de la partie privée : ");
					scanf("%d", &codePrive);

					rejoindrePartiePrivee(clientLocal, codePrive);
					break;
				case 3:
					clearScreen();

					printf("Entrez le nombre de joueurs requis pour lancer : ");
					scanf("%d", &nbJoueursPrive);

					creerPartiePrivee(clientLocal, nbJoueursPrive);
					break;
				case 4:
					exit(0);
					break;
			}
		}


		while (requete.code != COMMENCER_PARTIE && requete.code != ERREUR) {

			debugprintf("Attente de requête\n");
			recevoir(socketAppel, &requete, (pFct)deserialiserData);
			debugprintf("Requête reçue\n");

			switch (requete.code) {

				// Le serveur nous demande les informations en tant qu'hébergeur de partie publique
				case CREATION_PARTIE:
					socketEcouteHebergeur = creerSocketEcoute("127.0.0.1", 0);

					demandeCreation.isPrivate = 0;
					strcpy(demandeCreation.adresseHost, "127.0.0.1");
					demandeCreation.portHost = ntohs(socketEcouteHebergeur.adrLoc.sin_port);
					demandeCreation.nbJoueursMax = 2;
					demandeCreation.idClient = clientLocal.id;

					debugprintf("Création d'un serveur de jeu sur le port %d\n", demandeCreation.portHost);

					envoyerCreationPartie(socketAppel, demandeCreation);
					break;


					// Réception des informations d'un salon
				case SALON:
					deserialiserSalon(requete.data, &salon);
					if (salon.isPrivate) {
						printf("Code du salon : %d\n", salon.code);
					}
					break;
				case ERREUR:
					printf("Une erreur est survenue : %s\n", requete.data);
					break;

				case COMMENCER_PARTIE:
					partie = malloc(sizeof(Partie));

					deconnexionServeurUNO();

					/*
					SI pas host
						connection à l'host
						attendre recevoir partie
					Sinon
						attendre le nombre de connections attendu
						creation partie
					jouer partie
					*/
					debugprintf("Démarrage\n");
					if (salon.idHost == clientLocal.id) {
						debugprintf("Je suis HOST avec %d joueurs\n", salon.nbJoueursMax);
						clients = initConnection(salon);

						debugprintf("Je suis sur le port : %d\n", ntohs(socketEcouteHebergeur.adrLoc.sin_port));

						for (int i = 0; i < partie->nbJoueurs; i++) {
							debugprintf("client connecté sur port %d\n", clients[i].id, clients[i].port);
						}
						initPartie(partie, salon.nbJoueursMax, clients, clientLocal.id);

						debugprintf("Création partie faite avec %d joueurs\n\n", partie->nbJoueurs);


						reqEnvoiPartie(clients, partie);

						jouerPartieServeur(partie, clientLocal.id, clients);

						for (int i = 1; i < partie->nbJoueurs; i++) {
							CHECK(close(clients[i].socket.fd), "close socket client");
						}
						free(clients);

						CHECK(close(socketEcouteHebergeur.fd), "close socket hébergeur");
					}
					else {
						debugprintf("Je suis client\n");

						socketPartie = connecterClt2Srv(SOCK_STREAM, salon.adresseHost, salon.portHost);
						debugprintf("Connection serveur faite\n");

						resEnvoiPartie(socketPartie, partie);
						debugprintf("Reception partie faite %d\n", clientLocal.id);

						jouerPartieClient(partie, clientLocal.id, socketPartie);

						CHECK(close(socketPartie.fd), "close socket partie");
					}

					break;
				default:
					printf("CODE RECU NON RECONNU !\n");
			}

		}


	}

	return 0;
}

/**
 * Fonction à exécuter lors de la fermeture du programme
 */
void bye() {
	deconnexionServeurUNO();

	if (socketEcouteHebergeur.fd != -1) {
		debugprintf("Fermeture socket d'écoute d'hébergeur de partie\n");
		CHECK(close(socketEcouteHebergeur.fd), "close socket hébergeur");
		CHECK(close(socketPartie.fd), "close socket partie");
	}
}

/**
 * Connecte un client au serveur
 *
 * @return l'objet client
 */
client_t connexionServeurUNO() {
	basic_data_t requete = {-1, ""};
	client_t client;

	// Demande d’une connexion au service
	//PAUSE("Se connecter au service");
	socketAppel = connecterClt2Srv(SOCK_STREAM, ADRESSE_SVC, PORT_SVC);

	// Réception des informations client
	recevoir(socketAppel, &requete, (pFct)deserialiserData);
	if (requete.code == CLIENT) {
		deserialiserClient(requete.data, &client);
		debugprintf("Je suis le client n°%d, port:%d\n", client.id, client.port);
	}
	else {
		debugprintf("Erreur\n");
	}

	return client;
}

/**
 * Neutralise le ^C afin d'avoir une fermeture propre du programme
 */
void traiterSignal(int sigNum) {
	switch (sigNum) {
		case SIGINT:
			exit(0); // Sortie par ^C
			break;
	}
}

/**
 * Initialise la connection avec un salon du serveur
 * @param salon le salo du serveur
 *
 * @return l'objet client
 */
client_t* initConnection(salon_t salon) {
	client_t* clients = malloc((salon.nbJoueursMax) * sizeof(client_t));
	if (clients == NULL) {
		perror("Erreur allocation mémoire pour clients");
		exit(EXIT_FAILURE);
	}

	clients[0].socket.fd = -1;
	clients[0].id = salon.idHost;
	clients[0].port = salon.portHost;
	strcpy(clients[0].adresse, salon.adresseHost);

	int nbJoueursConnectes = 0;
	for (int i = 1; i < salon.nbJoueursMax; i++) {
		if (salon.idClients[i] != salon.idHost) {
			clients[i].socket = accepterClt(socketEcouteHebergeur);
			clients[i].id = salon.idClients[i];
			debugprintf("connecté à id%d\n", clients[i].id);
			clients[i].port = ntohs(clients[i].socket.adrDist.sin_port);
			strcpy(clients[i].adresse, inet_ntoa(clients[i].socket.adrDist.sin_addr));
			nbJoueursConnectes++;
		}

	}



	return clients;
}

/**
 * Déconnecte le client du serveur
 */
void deconnexionServeurUNO() {
	debugprintf("Envoi requête déconnexion au serveur\n");

	basic_data_t requete;
	requete.code = DECONNEXION;
	requete.data[0] = '\0';
	envoyer(socketAppel, &requete, (pFct)serialiserData);

	debugprintf("Fermeture socket appel\n");
	CHECK(close(socketAppel.fd), "close socket appel");
	connecte = 0;
}

/**
 * Le client quitte son salon actuel
 */
void quitterSalon() {
	debugprintf("Envoi requête quitter salon\n");

	basic_data_t requete;
	requete.code = QUITTER_PARTIE;
	requete.data[0] = '\0';
	envoyer(socketAppel, &requete, (pFct)serialiserData);
}

/**
 * Rejoint une partie publique
 */
void lancerPartiePublique(client_t clientLocal) {
	rejoindre_partie_t demandeRejoindre;
	demandeRejoindre.idClient = clientLocal.id;
	demandeRejoindre.isPrivate = 0;
	envoyerRejoindrePartie(socketAppel, demandeRejoindre);
}

/**
 * Rejoint une partie privée
 * @param clientLocal le client
 * @param code le code d'entrée de la partie
 */
void rejoindrePartiePrivee(client_t clientLocal, int code) {
	rejoindre_partie_t demandeRejoindre;
	demandeRejoindre.idClient = clientLocal.id;
	demandeRejoindre.isPrivate = 1;
	demandeRejoindre.code = code;
	envoyerRejoindrePartie(socketAppel, demandeRejoindre);
}

/**
 * Créé une partie privée
 */
void creerPartiePrivee(client_t clientLocal, int nbJoueursMax) {
	creation_partie_t demandeCreation;

	socketEcouteHebergeur = creerSocketEcoute("127.0.0.1", 0);

	demandeCreation.isPrivate = 1;
	strcpy(demandeCreation.adresseHost, "127.0.0.1");
	demandeCreation.portHost = ntohs(socketEcouteHebergeur.adrLoc.sin_port);
	demandeCreation.nbJoueursMax = nbJoueursMax;
	demandeCreation.idClient = clientLocal.id;

	debugprintf("Création d'un serveur de jeu sur le port %d\n", demandeCreation.portHost);

	envoyerCreationPartie(socketAppel, demandeCreation);
}

/**
 * Affiche le menu
 */
void afficherMenu() {
	setTerm(BLACK);
	printf("===== Menu Principal =====\n");
	resetTerm();
	printf("1. Lancer une partie publique\n");
	printf("2. Lancer une partie privée\n");
	printf("3. Héberger une partie privée\n");
	printf("4. Quitter\n");
	setTerm(BLACK);
	printf("==========================\n");
	resetTerm();
	printf("\nChoix : ");
}

