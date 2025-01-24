#ifndef LIBPSY
#define LIBSPY

// Pour avoir accès à sigaction, SA_RESTART (et probablement d'autres choses)
#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdarg.h>


/**
 * Installe un gestionnaire de signal (effectue une action à la réception d'un signal)
 * @param sigNum numéro du signal à gérer
 * @param handler fonction à appeler quand le signal est reçu
 */
void installSignal(int sigNum, void (*handler)(int));


/**
 * Bloque tous les signaux sauf SIGKILL et SIGSTOP (impossible à bloquer)
 * @warning SIGCHLD est bloqué aussi
 */
void bloquerSignaux(void);

/**
 * Débloque des signaux significatifs
 * @param nbSig nombre de signaux à débloquer
 * @param ... liste des signaux à débloquer
 */
void debloquerSignaux(int nbSig, ...);

typedef void (*pFctGenerique)(void*);
typedef void* generique;

/**
 * Crée un processus fils
 * @param pF fonction à exécuter par le fils
 * @return pid du fils
 */
pid_t cloner(pFctGenerique pF, generique arg);
#endif