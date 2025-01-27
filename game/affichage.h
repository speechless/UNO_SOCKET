#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "enhanceTerminal.h"
#include "game.h"


void afficherCarte(Carte c);
void afficherCarteMain(Carte c, int index);
void afficherMainAvecSelection(Joueur joueur, Carte carteVisible);
void afficherMenu(int* state, int input);
void afficherMain(Joueur j);

#endif // AFFICHAGE_H