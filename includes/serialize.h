/**
 * @file serialize.h
 */

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Pour va_list et les fonctions associées
#include "struct.h"

/**
 * Sérialise une partie
 * @param p Partie en cours
 * @param chaine Chaîne de caractères résultante
 */
void serialiserPartie(Partie* p, char* chaine);

/**
 * Sérialise une partie
 * @param p Partie en cours
 * @param chaine Chaîne de caractères résultante
 */
void deserialiserPartie(char* chaine, Partie* p);

/**
 * Sérialise la mise à jour d'une partie
 * @param p Partie en cours
 * @param chaine Chaîne de caractères résultante
 */
void serialiserCoup(Partie p, char* chaine);

/**
 * Désérialise la mise à jour d'une partie
 * @param chaine Chaîne de caractères résultante
 * @param p Partie en cours
 */
void deserialiserCoup(char* chaine, Partie* p);

#endif // SERIALIZE_H