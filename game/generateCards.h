#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "enhanceTerminal.h"


#ifndef CARTES_H
#define CARTES_H

void afficherCarte(Carte c);
void afficherCarteMain(Carte c, int index);
Carte *genererCartes();
void melangerCartes(Carte *cartes, int nbCartes);

void piocherCarte(Partie *partie, int idJoueur);
int isCarteVide(Carte c);


#endif // CARTES_H

