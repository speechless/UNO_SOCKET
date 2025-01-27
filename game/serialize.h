#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Pour va_list et les fonctions associées
#include "struct.h"

#define DEBUG 0

void serialiserPartie(Partie p, char* chaine);
void deserialiserPartie(const char* chaine, Partie* p);
void debugprintf(const char* format, ...);

#endif // SERIALIZE_H