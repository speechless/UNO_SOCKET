#ifndef COMMON
#define COMMON

// Vérifier la bonne exécution des [appels systèmes]?
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}

// Vérifier la bonne exécution des fonctions de thread
#define CHECK_ZERO(sts, msg) if ((sts) != 0) { fprintf(stderr, "pb lib thread %s\tligne %d", msg, __LINE__); exit(EXIT_FAILURE); }

#define PAUSE(msg) printf("%s [Entrée pour continuer]\n", msg); getchar();

#endif