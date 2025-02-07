#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "struct.h"
#include "enhanceTerminal.h"
#include "generateCards.h"
#include "affichage.h"
#include "serialize.h"
#include "game_requests.h"



void afficherMain(Joueur j);
int generateCode();

void initPartie(Partie* partie, int nbJoueurs, socket_t* sockets);
void initPartieClient(Partie* partie, int nbJoueurs);
int jouerCarte(Partie* partie, int idJoueur, Carte carteJouee);

void afficherMainAvecSelection(Joueur joueur, Carte carteVisible);

void prochainTour(Partie* partie);

#endif // GAME_H