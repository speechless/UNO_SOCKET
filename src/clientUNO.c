#include <session.h>
#include <data.h>
#include <enhanceTerminal.h>
#include <libPSY.h>

#include <inc.h>
#include <common.h>

#include <unistd.h>
#include <string.h>

#include <requetes.h>

void traiterSignal(int sigNum);
void bye();

socket_t socketAppel;
socket_t socketEcouteHebergeur;

/**
 * states :
 * 0 : Menu principal
 * 1 : Attente de partie publique
 * 2 : Entre le code de la partie privée
 * 3 : Attente de partie privée (hébergeur)
 * 4 : Attente de partie privée (rejoindre)
 *
 */
int state = 0;

int main() {


	salon_t salon;
	client_t clientLocal;

	basic_data_t requete = {-1, ""};
	creation_partie_t demandeCreation;
	rejoindre_partie_t demandeRejoindre;

	installSignal(SIGINT, traiterSignal);
	atexit(bye);

	// Demande d’une connexion au service
	PAUSE("Se connecter au service");
	socketAppel = connecterClt2Srv(SOCK_STREAM, ADRESSE_SVC, PORT_SVC);

	// Réception des informations client
	recevoir(socketAppel, &requete, (pFct)deserialiserData);
	if (requete.code == CLIENT) {
		deserialiserClient(requete.data, &clientLocal);
		fprintf(stderr, "Je suis le client n°%d\n", clientLocal.id);
	}
	else {
		printf("Erreur\n");
	}

	//TODO menu
	// Test statique de partie publique
	demandeRejoindre.idClient = clientLocal.id;
	demandeRejoindre.isPrivate = 0;
	envoyerRejoindrePartie(socketAppel, demandeRejoindre);

	while (requete.code != COMMENCER_PARTIE) {
		recevoir(socketAppel, &requete, (pFct)deserialiserData);

		switch (requete.code) {

			// Le serveur nous demande les informations en tant qu'hébergeur de partie publique
			case CREATION_PARTIE:
				socketEcouteHebergeur = creerSocketEcoute("127.0.0.1", 0);

				demandeCreation.isPrivate = 0;
				strcpy(demandeCreation.adresseHost, "127.0.0.1");
				demandeCreation.portHost = ntohs(socketEcouteHebergeur.adrLoc.sin_port);
				demandeCreation.nbJoueursMax = 2;
				demandeCreation.idClient = clientLocal.id;

				envoyerCreationPartie(socketAppel, demandeCreation);
				break;


				// Réception des informations d'un salon
			case SALON:
				deserialiserSalon(requete.data, &salon);
				break;
			case COMMENCER_PARTIE:
				printf("Démarrage\n");
		}
	}


	PAUSE("Fermer la socket d'appel");
	close(socketAppel.fd);


	return 0;
}

//TODO: envoyer deconnexion
void bye() {
	printf("Fermeture socket appel\n");
	CHECK(close(socketAppel.fd), "close socket appel");

	//printf("Fermeture des sockets de dialogue restantes\n");
}

void traiterSignal(int sigNum) {
	switch (sigNum) {
		case SIGINT:
			exit(0); // Sortie par ^C
			break;
	}
}

/*

void afficherMenu(int* state, int input) {

	switch (*state) {
		case 0:
			switch (input) {
				case 0:
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
					printf("\nInput : ");
					break;

				case 1:
					//Vue de la partie publique
					*state = 1;
					afficherMenu(state, 0);
					break;

				case 2:
					//Vue de la partie privée
					*state = 2;
					afficherMenu(state, 0);
					break;

				case 3:
					//Vue de l'hébergement de la partie privée
					*state = 3;
					afficherMenu(state, 0);
					break;

				case 4:
					exit(0);
					break;

				default:
					printf("Entrée invalide.\n");
					break;
			}
			break;

		case 1:
			//Vue de la partie publique
			setTerm(BLUE);
			printf("En attente de joueurs...\n");
			resetTerm();
			break;
		case 2:
			//Vue de la partie privée
			setTerm(BLACK);
			printf("Veuillez entrer le code de la partie : ");
			resetTerm();
			break;

		case 3:
			//Vue de l'hébergement de la partie privée
			setTerm(BLACK);
			printf("Code de la partie : %d\n", generateCode());
			resetTerm();
			break;

		default:
			exit(0);
	}
}*/