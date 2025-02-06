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

/**
 * Sérialise les données de base
 * @param data Données à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserData(basic_data_t* data, char* chaine);

/**
 * Désérialise les données de base
 * @param chaine Chaîne de caractères à désérialiser
 * @param data Données résultantes
 */
void deserialiserData(char* chaine, basic_data_t* data);

/**
 * Envoie un message d'erreur
 * @param socket Socket à utiliser pour l'envoi
 * @param message Message d'erreur à envoyer
 */
void envoyerErreur(socket_t socket, char* message);

/**
 * Sérialise un salon
 * @param salon Salon à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserSalon(salon_t* salon, char* chaine);

/**
 * Désérialise un salon
 * @param chaine Chaîne de caractères à désérialiser
 * @param salon Salon résultant
 */
void deserialiserSalon(char* chaine, salon_t* salon);

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
void initSalon(salon_t* salon, int id, int isPrivate, int idHost, char* adresseHost, short portHost, int nbJoueursActuels, int nbJoueursMax);

/**
 * Envoie les informations d'un salon
 * @param socket Socket à utiliser pour l'envoi
 * @param salon Salon à envoyer
 */
void envoyerSalon(socket_t socket, salon_t salon);

/**
 * Affiche les informations d'un salon
 * @param salon Salon à afficher
 */
void afficherSalon(salon_t salon);

/**
 * Sérialise une demande de création de partie
 * @param demande Demande à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserCreationPartie(creation_partie_t* demande, char* chaine);

/**
 * Désérialise une demande de création de partie
 * @param chaine Chaîne de caractères à désérialiser
 * @param demande Demande résultante
 */
void deserialiserCreationPartie(char* chaine, creation_partie_t* demande);

/**
 * Envoie une demande de création de partie
 * @param socket Socket à utiliser pour l'envoi
 * @param demande Demande à envoyer
 */
void envoyerCreationPartie(socket_t socket, creation_partie_t demande);

/**
 * Sérialise une demande de rejoindre une partie
 * @param demande Demande à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserRejoindrePartie(rejoindre_partie_t* demande, char* chaine);

/**
 * Désérialise une demande de rejoindre une partie
 * @param chaine Chaîne de caractères à désérialiser
 * @param demande Demande résultante
 */
void deserialiserRejoindrePartie(char* chaine, rejoindre_partie_t* demande);

/**
 * Envoie une demande de rejoindre une partie
 * @param socket Socket à utiliser pour l'envoi
 * @param demande Demande à envoyer
 */
void envoyerRejoindrePartie(socket_t socket, rejoindre_partie_t demande);

/**
 * Sérialise un client
 * @param client Client à sérialiser
 * @param chaine Chaîne de caractères résultante
 */
void serialiserClient(client_t* client, char* chaine);

/**
 * Désérialise un client
 * @param chaine Chaîne de caractères à désérialiser
 * @param client Client résultant
 */
void deserialiserClient(char* chaine, client_t* client);

#endif