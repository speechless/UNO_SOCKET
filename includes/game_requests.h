#ifndef REQ_GAME_H
#define REQ_GAME_H

#include "serialize.h"
#include "struct.h"
#include <stdlib.h>
#include <data.h>

void reqEnvoiPartie(socket_t* sockets, Partie* partie);
void resEnvoiPartie(socket_t socket, Partie* partie);

void reqEnvoiCoupServeur(socket_t* sockets, Partie partie);
void reqEnvoiCoupClient(socket_t socketHost, Partie partie);
void resEnvoiCoup(socket_t socket, Partie partie);

void envoiTest(socket_t socket, int* test);

void recevoirTest(socket_t socket, int* test);

int serialiserD(char* chaine, void* data);

int deserialiserD(char* chaine, void* data);
#endif // REQ_GAME_H