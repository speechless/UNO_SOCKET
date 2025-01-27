#include <session.h>
#include <data.h>

#include <inc.h>
#include <common.h>

#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <requetes.h>


int main() {
	socket_t socketAppel;
	basic_data_t requete;
	salon_t salon;
	demande_salon demande;

	// Demande d’une connexion au service
	PAUSE("Se connecter au service");
	socketAppel = connecterClt2Srv(SOCK_STREAM, ADRESSE_SVC, PORT_SVC);

	demande.isPrivate = 0;
	demande.nbJoueursMax = 3;

	requete.code = DEMANDE_SALON;
	serialiserDemandeSalon(&demande, requete.data);

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