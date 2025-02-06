/**
 * @file data.h
 */

#include "session.h"

 /**
  * Définition du type de données generic
  */
typedef void* generic;

/**
 * Définition d'un pointeur de fonction avec deux arguments generic
 */
typedef void (*pFct)(generic, generic);

/**
 * @brief 			Envoyer des données sur une socket
 * @param sock 		Socket sur laquelle envoyer les données
 * @param quoi 		Pointeur sur les données à envoyer
 * @param serial 	Fonction de sérialisation à utiliser
 * @param ... 		Paramètres supplémentaire si mode DGRAM (adrIP(str) puis port(short))
 * @note 			Si quoi est une chaine de caractères alors serial sera NULL
 */
void envoyer(socket_t sock, generic quoi, pFct serial, ...);

/**
 * @brief 			Recevoir des données depuis une socket
 * @param sock 		Socket sur laquelle recevoir les données
 * @param quoi 		Pointeur sur l'endroit où recevoir les données
 * @param deserial 	Fonction de désérialisation à utiliser
 * @note 			Si les données reçues sont sous forme de chaine de caractère, alors deserial sera NULL
 */
void recevoir(socket_t sock, generic quoi, pFct deserial);

