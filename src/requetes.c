/**
 * @file requetes.c
 */
#include <requetes.h>
#include <stdio.h>
#include <data.h>
#include <string.h>

 /**
  * Sérialise les données de base
  * @param data Données à sérialiser
  * @param chaine Chaîne de caractères résultante
  */
void serialiserData(basic_data_t* data, char* chaine) {
	sprintf(chaine, "[%d] : %s", data->code, data->data);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

/**
 * Désérialise les données de base
 * @param chaine Chaîne de caractères à désérialiser
 * @param data Données résultantes
 */
void deserialiserData(char* chaine, basic_data_t* data) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "[%d] : %[\001-\377]", &data->code, data->data);
	//fprintf(stderr, "Désérialisé : #[%d] : %s#\n", data->code, data->data);
}

/**
 * Envoie un message d'erreur
 * @param socket Socket à utiliser pour l'envoi
 * @param message Message d'erreur à envoyer
 */
void envoyerErreur(socket_t socket, char* message) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = ERREUR;
	strcpy(requete.data, message);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

/**
 * Sérialise un salon
 * @param salon Salon à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserSalon(salon_t* salon, char* chaine) {
	char buffer[50];
	afficherSalon(*salon);

	char chaineIdJoueurs[20] = "";
	for (int i = 0; i < salon->nbJoueursActuels; i++) {
		sprintf(buffer, "%d:", salon->idClients[i]);
		strcat(chaineIdJoueurs, buffer);
	}

	sprintf(chaine, "%d:%d:%d:%s:%d:%d:%d:[%s]:%d", salon->id, salon->isPrivate, salon->idHost, salon->adresseHost, salon->portHost, salon->nbJoueursActuels, salon->nbJoueursMax, chaineIdJoueurs, salon->code);
	fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

/**
 * Désérialise un salon
 * @param chaine Chaîne de caractères à désérialiser
 * @param salon Salon résultant
 */
void deserialiserSalon(char* chaine, salon_t* salon) {
	fprintf(stderr, "recu : #%s#\n", (char*)chaine);
	char chaineIdJoueurs[20];

	sscanf(chaine, "%d:%d:%d:%[^:]:%hd:%d:%d:[%[^]]]:%d", &salon->id, &salon->isPrivate, &salon->idHost, salon->adresseHost, &salon->portHost, &salon->nbJoueursActuels, &salon->nbJoueursMax, chaineIdJoueurs, &salon->code);

	char* ptr = chaineIdJoueurs;
	for (int i = 0; i < salon->nbJoueursActuels; i++) {
		sscanf(ptr, "%d:", &salon->idClients[i]);
		ptr = strchr(ptr, ':') + 1;
	}

	afficherSalon(*salon);
}

/**
 * Initialise un salon
 * @param salon Salon à initialiser
 * @param id Identifiant du salon
 * @param isPrivate 1 si le salon est privé, 0 sinon
 * @param idHost Identifiant de l'hôte du salon
 * @param adresseHost Adresse de l'hôte du salon
 * @param portHost Port de l'hôte du salon
 * @param nbJoueursActuels Nombre de joueurs actuels dans le salon
 * @param nbJoueursMax Nombre de joueurs maximum dans le salon
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

/**
 * Envoie les informations d'un salon
 * @param socket Socket à utiliser pour l'envoi
 * @param salon Salon à envoyer
 */
void envoyerSalon(socket_t socket, salon_t salon) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = SALON;
	serialiserSalon(&salon, requete.data);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

/**
 * Affiche les informations d'un salon
 * @param salon Salon à afficher
 */
void afficherSalon(salon_t salon) {
	printf("Salon.id = %d\n", salon.id);
	printf("Salon.isPrivate = %d\n", salon.isPrivate);
	printf("Salon.idHost = %d\n", salon.idHost);
	printf("Salon.adresseHost = %s\n", salon.adresseHost);
	printf("Salon.portHost = %d\n", salon.portHost);
	printf("Salon.joueursActuel = %d\n", salon.nbJoueursActuels);
	printf("Salon.joueursMax = %d\n", salon.nbJoueursMax);
	printf("Salon.idJoueurs = [");
	for (int i = 0; i < salon.nbJoueursActuels; i++) {
		printf("%d, ", salon.idClients[i]);
	}
	printf("]\n");
	printf("Salon.code = %d\n", salon.code);
}

/**
 * Sérialise une demande de création de partie
 * @param demande Demande à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserCreationPartie(creation_partie_t* demande, char* chaine) {
	sprintf(chaine, "%d:%d:%d:%s:%d", demande->idClient, demande->isPrivate, demande->nbJoueursMax, demande->adresseHost, demande->portHost);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

/**
 * Désérialise une demande de création de partie
 * @param chaine Chaîne de caractères à désérialiser
 * @param demande Demande résultante
 */
void deserialiserCreationPartie(char* chaine, creation_partie_t* demande) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%d:%[^:]:%hd", &demande->idClient, &demande->isPrivate, &demande->nbJoueursMax, demande->adresseHost, &demande->portHost);
}

/**
 * Envoie une demande de création de partie
 * @param socket Socket à utiliser pour l'envoi
 * @param demande Demande à envoyer
 */
void envoyerCreationPartie(socket_t socket, creation_partie_t demande) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = CREATION_PARTIE;
	serialiserCreationPartie(&demande, requete.data);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

/**
 * Sérialise une demande de rejoindre une partie
 * @param demande Demande à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserRejoindrePartie(rejoindre_partie_t* demande, char* chaine) {
	sprintf(chaine, "%d:%d:%d", demande->idClient, demande->isPrivate, demande->code);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

/**
 * Désérialise une demande de rejoindre une partie
 * @param chaine Chaîne de caractères à désérialiser
 * @param demande Demande résultante
 */
void deserialiserRejoindrePartie(char* chaine, rejoindre_partie_t* demande) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%d", &demande->idClient, &demande->isPrivate, &demande->code);
}

/**
 * Envoie une demande de rejoindre une partie
 * @param socket Socket à utiliser pour l'envoi
 * @param demande Demande à envoyer
 */
void envoyerRejoindrePartie(socket_t socket, rejoindre_partie_t demande) {
	basic_data_t requete;

	// Préparation de la requête
	requete.code = REJOINDRE_PARTIE;
	serialiserRejoindrePartie(&demande, requete.data);

	// Envoi des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

/**
 * Sérialise un client
 * @param client Client à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserClient(client_t* client, char* chaine) {
	sprintf(chaine, "%d:%s:%d", client->id, client->adresse, client->port);
	//fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

/**
 * Désérialise un client
 * @param chaine Chaîne de caractères à désérialiser
 * @param client Client résultant
 */
void deserialiserClient(char* chaine, client_t* client) {
	//fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%[^:]:%hd", &client->id, client->adresse, &client->port);
}