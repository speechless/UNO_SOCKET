#ifdef DEBUG
#define DEBUG_PRINT 1
#else
#define DEBUG_PRINT 0
#endif


#include <stdio.h>
#include <string.h>
#include <stdarg.h> // Pour va_list et les fonctions associées

void debugprintf(char* format, ...) {
	if (DEBUG_PRINT) {
		va_list args;                // Initialise une liste d'arguments variadiques
		va_start(args, format);      // Associe la liste avec les arguments passés à la fonction
		vfprintf(stderr, format, args);       // Appelle printf avec les arguments
		va_end(args);                // Termine l'utilisation des arguments variadiques
	}
}