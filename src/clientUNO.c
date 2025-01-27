#include <session.h>
#include <data.h>

#include <inc.h>
#include <common.h>

#include <unistd.h>

#include <requetes.h>


int main() {
	socket_t socketAppel;
	basic_data_t requete = {100, "J'envoie une requête de code 100"};
	basic_data_t reponse;

	// Demande d’une connexion au service
	PAUSE("Se connecter au service");
	socketAppel = connecterClt2Srv(SOCK_STREAM, ADRESSE_SVC, PORT_SVC);

	PAUSE("Envoyer un message au serveur");
	envoyer(socketAppel, &requete, (pFct)serialiserData);

	PAUSE("Lire un message du serveur");
	recevoir(socketAppel, &reponse, (pFct)deserialiserData);

	PAUSE("Fermer la socket d'appel");
	close(socketAppel.fd);


	return 0;
}