#ifndef REQUETES
#define REQUETES

#define MAX_BUFF 1024

#include <session.h>

/**
 * Type de requête
 */
typedef enum {
	DECONNEXION,
	QUITTER_PARTIE,
	CREATION_PARTIE,
	REJOINDRE_PARTIE,
	COMMENCER_PARTIE,
	SALON,
	CLIENT,
	ERREUR,
} type_requete;

/**
 * Structure représentant un client
 */
typedef struct {
	int id;
	socket_t socket;
	char adresse[20];
	unsigned short port;
} client_t;

/**
 * Structure représentant une requête basique
 */
typedef struct {
	int code;
	char data[MAX_BUFF];
} basic_data_t;

/**
 * Structure représentant un salon pour la mise en relation des joueurs
 */
typedef struct {
	int id;
	int isPrivate;
	int idHost;
	char adresseHost[20];
	unsigned short portHost;
	int nbJoueursActuels;
	int nbJoueursMax;
	int idClients[20];
	int code;
} salon_t;

/**
 * Structure contenant les informations pour la création d'une partie
 */
typedef struct {
	int idClient;
	int isPrivate;
	int nbJoueursMax;
	char adresseHost[20];
	unsigned short portHost;
} creation_partie_t;

/**
 * Structure contenant les informations pour rejoindre une partie
 */
typedef struct {
	int idClient;
	int isPrivate;
	int code;
} rejoindre_partie_t;



void serialiserData(basic_data_t* data, char* chaine);
void deserialiserData(char* chaine, basic_data_t* data);
void envoyerErreur(socket_t socket, char* message);

void serialiserSalon(salon_t* salon, char* chaine);
void deserialiserSalon(char* chaine, salon_t* salon);
void initSalon(salon_t* salon, int id, int isPrivate, int isHost, char* adresseHost, short portHost, int nbJoueursActuels, int nbJoueursMax);
void envoyerSalon(socket_t socket, salon_t salon);
void afficherSalon(salon_t salon);

void serialiserCreationPartie(creation_partie_t* demande, char* chaine);
void deserialiserCreationPartie(char* chaine, creation_partie_t* demande);
void envoyerCreationPartie(socket_t socket, creation_partie_t demande);

void serialiserRejoindrePartie(rejoindre_partie_t* demande, char* chaine);
void deserialiserRejoindrePartie(char* chaine, rejoindre_partie_t* demande);
void envoyerRejoindrePartie(socket_t socket, rejoindre_partie_t demande);

void serialiserClient(client_t* client, char* chaine);
void deserialiserClient(char* chaine, client_t* client);
#endif