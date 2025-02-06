/**
 * TODO:
 * menu
 * connexion entre clients quand la partie est lancée
 * jeu
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

void traiterSignal(int sigNum);
void bye();
void deconnexionServeurUNO();
client_t connexionServeurUNO();
void quitterSalon();
//void afficherMenu(int* state, int input);
void lancerPartiePublique(client_t clientLocal);

socket_t socketAppel;
socket_t socketEcouteHebergeur = {-1,};

/**
 * states :
 * 0 : Menu principal
 * 1 : Attente de partie publique
 * 2 : Entre le code de la partie privée
 * 3 : Attente de partie privée (hébergeur)
 * 4 : Attente de partie privée (rejoindre)
 */
int state = 0;

int main() {

	salon_t salon;
	client_t clientLocal;

	basic_data_t requete = {-1, ""};
	creation_partie_t demandeCreation;
	rejoindre_partie_t demandeRejoindre;

	struct timeval to = {0,0};
	fd_set read_fs;

	installSignal(SIGINT, traiterSignal);
	atexit(bye);


	// Connexion au hub de jeu
	clientLocal = connexionServeurUNO();

	// Test statique de partie publique
	lancerPartiePublique(clientLocal);

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

				printf("Création d'un serveur de jeu sur le port %d\n", demandeCreation.portHost);

				envoyerCreationPartie(socketAppel, demandeCreation);
				break;


				// Réception des informations d'un salon
			case SALON:
				deserialiserSalon(requete.data, &salon);
				break;
			case COMMENCER_PARTIE:
				/*
				SI pas host 
					connection à l'host
					attendre recevoir partie
				Sinon 
					attendre le nombre de connections attendu
					creation partie
				jouer partie
				*/
				printf("Démarrage\n");
<<<<<<< Updated upstream
				break;
			default:
				printf("CODE RECU NON RECONNU !\n");

=======
				if(salon.idHost == clientLocal.id){
					initConnection(salon);
					Partie p = initPartie();
					for(int i = 0; i < salon.nbJoueursMax; i++){
						reqEnvoiPartie(salon.idClients[i],p);
					}
				}else{
					
				}
				jouerPartie(p);
				
>>>>>>> Stashed changes
		}
	}


	return 0;
}

void bye() {
	deconnexionServeurUNO();

	if (socketEcouteHebergeur.fd != -1) {
		fprintf(stderr, "Fermeture socket d'écoute d'hébergeur de partie\n");
		CHECK(close(socketEcouteHebergeur.fd), "close socket hébergeur");
	}
}

client_t connexionServeurUNO() {
	basic_data_t requete = {-1, ""};
	client_t client;

	// Demande d’une connexion au service
	PAUSE("Se connecter au service");
	socketAppel = connecterClt2Srv(SOCK_STREAM, ADRESSE_SVC, PORT_SVC);

	// Réception des informations client
	recevoir(socketAppel, &requete, (pFct)deserialiserData);
	if (requete.code == CLIENT) {
		deserialiserClient(requete.data, &client);
		fprintf(stderr, "Je suis le client n°%d, port:%d\n", client.id, client.port);
	}
	else {
		printf("Erreur\n");
	}

	return client;
}

void traiterSignal(int sigNum) {
	switch (sigNum) {
		case SIGINT:
			exit(0); // Sortie par ^C
			break;
	}
}

void initConnection(salon_t salon){
	int nbJoueursConnectes = 0;
	int nbJoueursMax = salon.nbJoueursMax;

	for(int i = 0; i < salon.nbJoueursMax; i++){
		accepterClt(salon.idClients[i]);
		nbJoueursConnectes++;
	}
}

void deconnexionServeurUNO() {
	fprintf(stderr, "Envoi requête déconnexion au serveur\n");

	basic_data_t requete;
	requete.code = DECONNEXION;
	requete.data[0] = '\0';
	envoyer(socketAppel, &requete, (pFct)serialiserData);

	printf("Fermeture socket appel\n");
	CHECK(close(socketAppel.fd), "close socket appel");
}

void quitterSalon() {
	fprintf(stderr, "Envoi requête quitter salon\n");

	basic_data_t requete;
	requete.code = QUITTER_PARTIE;
	requete.data[0] = '\0';
	envoyer(socketAppel, &requete, (pFct)serialiserData);
}

void lancerPartiePublique(client_t clientLocal) {
	rejoindre_partie_t demandeRejoindre;
	demandeRejoindre.idClient = clientLocal.id;
	demandeRejoindre.isPrivate = 0;
	envoyerRejoindrePartie(socketAppel, demandeRejoindre);
}

void rejoindrePartiePrivee(client_t clientLocal, int code) {
	rejoindre_partie_t demandeRejoindre;
	demandeRejoindre.idClient = clientLocal.id;
	demandeRejoindre.isPrivate = 1;
	demandeRejoindre.code = code;
	envoyerRejoindrePartie(socketAppel, demandeRejoindre);
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
			printf("Code de la partie : ...\n");
			resetTerm();
			break;

		default:
			exit(0);
	}
}*/

