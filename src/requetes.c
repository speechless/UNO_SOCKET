#include <requetes.h>
#include <stdio.h>
#include <data.h>
#include <string.h>

void serialiserData(basic_data_t* data, char* chaine) {
	sprintf(chaine, "[%d] : %s", data->code, data->data);
	fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserData(char* chaine, basic_data_t* data) {
	fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "[%d] : %[\001-\377]", &data->code, data->data);
	fprintf(stderr, "Désérialisé : #[%d] : %s#\n", data->code, data->data);
}


void serialiserSalon(salon_t* salon, char* chaine) {
	afficherSalon(*salon);
	sprintf(chaine, "%d:%d:%d:%s:%d:%d:%d", salon->id, salon->isPrivate, salon->isHost, salon->adresseHost, salon->portHost, salon->nbJoueursActuels, salon->nbJoueursMax);
	fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserSalon(char* chaine, salon_t* salon) {
	fprintf(stderr, "recu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%d:%[^:]:%hd:%d:%d", &salon->id, &salon->isPrivate, &salon->isHost, salon->adresseHost, &salon->portHost, &salon->nbJoueursActuels, &salon->nbJoueursMax);
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
void initSalon(salon_t* salon, int id, int isPrivate, int isHost, char* adresseHost, short portHost, int nbJoueursActuels, int nbJoueursMax) {
	salon->id = id;
	salon->isPrivate = isPrivate;
	salon->isHost = isHost;
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

	// Envoie des informations du salon au client
	envoyer(socket, &requete, (pFct)serialiserData);
}

void afficherSalon(salon_t salon) {
	printf("Salon.id = %d\n", salon.id);
	printf("Salon.isPrivate = %d\n", salon.isPrivate);
	printf("Salon.isHost = %d\n", salon.isHost);
	printf("Salon.adresseHost = %s\n", salon.adresseHost);
	printf("Salon.portHost = %d\n", salon.portHost);
	printf("Salon.joueursActuel = %d\n", salon.nbJoueursActuels);
	printf("Salon.joueursMax = %d\n", salon.nbJoueursMax);
}


void serialiserDemandeSalon(demande_salon* demande, char* chaine) {
	sprintf(chaine, "%d:%d:%s", demande->isPrivate, demande->nbJoueursMax, demande->codeSalon);
	fprintf(stderr, "envoi : #%s#\n", (char*)chaine);
}

void deserialiserDemandeSalon(char* chaine, demande_salon* demande) {
	fprintf(stderr, "Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "%d:%d:%s", &demande->isPrivate, &demande->nbJoueursMax, demande->codeSalon);
}