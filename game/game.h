#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "struct.h"
#include "enhanceTerminal.h"
#include "generateCards.h"
#include "affichage.h"


void afficherMenu(int* state,int input);
void afficherMain(Joueur j);
int generateCode();

Partie initPartie(int nbJoueurs);
int jouerCarte(Partie *partie, int idJoueur, Carte carteJouee);

void afficherMainAvecSelection(Joueur joueur, Carte carteVisible);

void prochainTour(Partie *partie);

#endif // GAME_H