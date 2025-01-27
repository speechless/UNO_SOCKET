#ifndef SESSION
#define SESSION

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

/**
 * @struct Structure représentant une socket
 */
struct socket {
	int fd; /**< file decriptor de la socket */
	int mode;
	struct sockaddr_in adrDist;
	struct sockaddr_in adrLoc;
};

/**
 * @typedef Définition du type de données socket_t
 */
typedef struct socket socket_t;

/**
 * @brief		Création d'une socket de type DGRAM/STREAM
 * @param mode	Mode d'ouverture de la socket DGRAM/STREAM
 * @result		Socket créée
 */
socket_t creerSocket(int mode);

/**
 * @brief		Création d'une socket adressée de type DGRAM/STREAM
 * @param mode 	Mode d'ouverture de la socket DGRAM/STREAM
 * @param adrIP Adresse IP de la socket
 * @param port 	Port sur lequel ouvrir la socket
 * @result		Socket créée
 */
socket_t creerSocketAdr(int mode, char* adrIP, unsigned short port);

/**
 * @brief		Création d'une socket d'écoute avec l'adressage fourni en paramétre
 * @param adrIP Adresse IP d'écoute
 * @param port 	Port sur lequel écouter
 * @result		Socket d'écoute créée
 */
socket_t creerSocketEcoute(char* adrIP, unsigned short port);

/**
 * @brief				Accepte la demande de connexion d'un client
 * @param sockEcoute 	Socket d'écoute sur laquelle accepter la connexion
 * @result				Socket de dialogue créée
 */
socket_t accepterClt(const socket_t sockEcoute);

/**
 * @brief				Création d'une socket d'appel et connexion au serveur dont l'adressage est fourni en paramètre
 * @param mode 	Mode d'ouverture de la socket DGRAM/STREAM
 * @param adrIP 		Adresse IP à laquelle se connecter
 * @param port 			Port sur lequel se connecter
 * @result				Socket d'appel créée
 */
socket_t connecterClt2Srv(int mode, char* adrIP, unsigned short port);

#endif