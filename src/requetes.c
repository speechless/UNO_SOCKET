#include <requetes.h>
#include <stdio.h>
#include <data.h>
#include <string.h>

void serialiserData(basic_data_t* data, char* chaine) {
	sprintf(chaine, "[%d] : %s", data->code, data->data);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserData(char* chaine, basic_data_t* data) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "[%d] : %[\001-\377]", &data->code, data->data);
	//fprintf(stderr, "Désérialisé : #[%d] : %s#\n", data->code, data->data);
}

void envoyerErreur(socket_t socket, char* message) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = ERREUR;
	strcpy(requete.data, message);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}


void serialiserSalon(salon_t* salon, char* chaine) {
	afficherSalon(*salon);
	sprintf(chaine, "%d:%d:%d:%s:%d:%d:%d:%s", salon->id, salon->isPrivate, salon->idHost, salon->adresseHost, salon->portHost, salon->nbJoueursActuels, salon->nbJoueursMax, salon->code);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserSalon(char* chaine, salon_t* salon) {
	//fprintf(stderr, "recu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%d:%[^:]:%hd:%d:%d:%s", &salon->id, &salon->isPrivate, &salon->idHost, salon->adresseHost, &salon->portHost, &salon->nbJoueursActuels, &salon->nbJoueursMax, salon->code);
	afficherSalon(*salon);
}

/**
 * Initialise un salon
 * @param salon : le salon à initialiser
 * @param id : l'identifiant du salon
 * @param isPrivate : 1 si le salon est privé, 0 sinon
 * @param isHost : 1 si le client est l'hôte du salon, 0 sinon
 * @param adresseHost : l'adresse de l'hôte du salon
 * @param portHost : le port de l'hôte du salon
 * @param nbJoueursActuels : le nombre de joueurs actuels dans le salon
 * @param nbJoueursMax : le nombre de joueurs maximum dans le salon
 */
void initSalon(salon_t* salon, int id, int isPrivate, int idHost, char* adresseHost, short portHost, int nbJoueursActuels, int nbJoueursMax) {
	salon->id = id;
	salon->isPrivate = isPrivate;
	salon->idHost = idHost;
	strcpy(salon->adresseHost, adresseHost);
	salon->portHost = portHost;
	salon->nbJoueursActuels = nbJoueursActuels;
	salon->nbJoueursMax = nbJoueursMax;
}

void envoyerSalon(socket_t socket, salon_t salon) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = SALON;
	serialiserSalon(&salon, requete.data);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

void afficherSalon(salon_t salon) {
	printf("Salon.id = %d\n", salon.id);
	printf("Salon.isPrivate = %d\n", salon.isPrivate);
	printf("Salon.idHost = %d\n", salon.idHost);
	printf("Salon.adresseHost = %s\n", salon.adresseHost);
	printf("Salon.portHost = %d\n", salon.portHost);
	printf("Salon.joueursActuel = %d\n", salon.nbJoueursActuels);
	printf("Salon.joueursMax = %d\n", salon.nbJoueursMax);
	printf("Salon.code = %s\n", salon.code);
}


void serialiserCreationPartie(creation_partie_t* demande, char* chaine) {
	sprintf(chaine, "%d:%d:%d:%s:%d", demande->idClient, demande->isPrivate, demande->nbJoueursMax, demande->adresseHost, demande->portHost);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserCreationPartie(char* chaine, creation_partie_t* demande) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%d:%[^:]:%hd", &demande->idClient, &demande->isPrivate, &demande->nbJoueursMax, demande->adresseHost, &demande->portHost);
}

void envoyerCreationPartie(socket_t socket, creation_partie_t demande) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = CREATION_PARTIE;
	serialiserCreationPartie(&demande, requete.data);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

void serialiserRejoindrePartie(rejoindre_partie_t* demande, char* chaine) {
	sprintf(chaine, "%d:%d:%s", demande->idClient, demande->isPrivate, demande->code);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserRejoindrePartie(char* chaine, rejoindre_partie_t* demande) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%s", &demande->idClient, &demande->isPrivate, demande->code);
}

void envoyerRejoindrePartie(socket_t socket, rejoindre_partie_t demande) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = REJOINDRE_PARTIE;
	serialiserRejoindrePartie(&demande, requete.data);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}


void serialiserClient(client_t* client, char* chaine) {
	sprintf(chaine, "%d:%s:%d", client->id, client->adresse, client->port);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserClient(char* chaine, client_t* client) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%[^:]:%hd", &client->id, client->adresse, &client->port);
}