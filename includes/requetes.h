/**
 * @file requetes.h
 */
#ifndef REQUETES
#define REQUETES

 /**
  * Buffer maximal pour les données
  */
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
	int id; /**< Identifiant du client */
	socket_t socket; /**< Socket du client */
	char adresse[20]; /**< Adresse du client en notation pointée */
	unsigned short port; /**< Port du client */
} client_t;

/**
 * Structure représentant une requête basique
 */
typedef struct {
	int code; /**< Code de la requête */
	char data[MAX_BUFF]; /**< Données de la requête */
} basic_data_t;

/**
 * Structure représentant un salon pour la mise en relation des joueurs
 */
typedef struct {
	int id; /**< Identifiant du salon */
	int isPrivate; /**< Indicateur de salon privé (1) ou public (0) */
	int idHost; /**< Identifiant de l'hôte du salon */
	char adresseHost[20]; /**< Adresse de l'hôte du salon en notation pointée */
	unsigned short portHost; /**< Port de l'hôte du salon */
	int nbJoueursActuels; /**< Nombre de joueurs actuels dans le salon */
	int nbJoueursMax; /**< Nombre de joueurs maximum dans le salon */
	int idClients[20]; /**< Identifiants des clients dans le salon */
	int code; /**< Code du salon s'il est privé, 0 sinon */
} salon_t;

/**
 * Structure contenant les informations pour la création d'une partie
 */
typedef struct {
	int idClient; /**< Identifiant du client */
	int isPrivate; /**< Indicateur de partie privée (1) ou publique (0) */
	int nbJoueursMax; /**< Nombre de joueurs maximum */
	char adresseHost[20]; /**< Adresse de l'hôte en notation pointée */
	unsigned short portHost; /**< Port de l'hôte */
} creation_partie_t;

/**
 * Structure contenant les informations pour rejoindre une partie
 */
typedef struct {
	int idClient; /**< Identifiant du client */
	int isPrivate; /**< Indicateur de partie privée (1) ou publique (0) */
	int code; /**< Code de la partie */
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