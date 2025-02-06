#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Pour va_list et les fonctions associées
#include "struct.h"

#define DEBUG 1

void serialiserPartie(Partie p, char* chaine);
void deserialiserPartie(char* chaine, Partie* p);

void serialiserCoup(Partie p ,char* chaine);
void deserialiserCoup(char* chaine, Partie* p);

void debugprintf(char* format, ...);

#endif // SERIALIZE_H