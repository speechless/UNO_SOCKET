#ifndef CARTES_H
#define CARTES_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "struct.h"

Carte *genererCartes();
void melangerCartes(Carte *cartes, int nbCartes);

void piocherCarte(Partie *partie, int idJoueur);
int isCarteVide(Carte c);


#endif // CARTES_H

