#ifndef REQ_GAME_H
#define REQ_GAME_H

#include "serialize.h"
#include "struct.h"
#include <stdlib.h>

void reqEnvoiPartie(/*int* sockets,*/ Partie partie);
void resEnvoiPartie(/*int socket,*/ Partie partie);

void reqEnvoiCoup(/*int* sockets,*/ Partie partie);
void resEnvoiCoup(/*int socket,*/ Partie partie);

#endif // REQ_GAME_H