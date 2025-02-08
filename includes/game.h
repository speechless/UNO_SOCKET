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
#include <requetes.h>



void afficherMain(Joueur j);

void initPartie(Partie* partie, int nbJoueurs, client_t* clients, int idHost);
int jouerCarte(Partie* partie, int idJoueur, Carte carteJouee);

void afficherMainAvecSelection(Joueur joueur, Carte carteVisible);

void prochainTour(Partie* partie);

int getIndexFromIdClient(int id, client_t* joueurs, int nbJoueurs);
int getIndexFromIdJoueur(int id, Joueur* joueurs, int nbJoueurs);

#endif // GAME_H