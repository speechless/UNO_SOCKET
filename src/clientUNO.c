#include <session.h>
#include <data.h>
#include <enhanceTerminal.h>

#include <inc.h>
#include <common.h>

#include <unistd.h>
#include <string.h>

#include <requetes.h>

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
	socket_t socketAppel;
	basic_data_t requete;
	salon_t salon;
	creation_partie_t demande;
	client_t clientLocal;

	// Demande d’une connexion au service
	PAUSE("Se connecter au service");
	socketAppel = connecterClt2Srv(SOCK_STREAM, ADRESSE_SVC, PORT_SVC);

	recevoir(socketAppel, &requete, (pFct)deserialiserData);
	if (requete.code == CLIENT) {
		deserialiserClient(requete.data, &clientLocal);
	}
	else {
		printf("Erreur\n");
	}

	demande.isPrivate = 0;
	strcpy(demande.adresseHost, ADRESSE_SVC);
	demande.portHost = PORT_SVC;
	demande.nbJoueursMax = 3;
	demande.idClient = clientLocal.id;

	requete.code = CREATION_PARTIE;
	serialiserCreationPartie(&demande, requete.data);

	PAUSE("Envoyer demande création partie publique");
	envoyer(socketAppel, &requete, (pFct)serialiserData);

	PAUSE("Recevoir le salon");
	recevoir(socketAppel, &requete, (pFct)deserialiserData);
	deserialiserSalon(requete.data, &salon);
	/*
	PAUSE("Envoyer un message au serveur");
	envoyer(socketAppel, &requete, (pFct)serialiserData);

	PAUSE("Lire un message du serveur");
	recevoir(socketAppel, &reponse, (pFct)deserialiserData);
*/
	PAUSE("Fermer la socket d'appel");
	close(socketAppel.fd);


	return 0;
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