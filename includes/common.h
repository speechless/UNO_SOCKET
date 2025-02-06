/**
 * @file common.h
 */

#ifndef COMMON
#define COMMON

 /**
  * Vérifier la bonne exécution des fonctions POSIX
  */
#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}

  /**
   * Vérifier la bonne exécution des fonctions de thread
   */
#define CHECK_ZERO(sts, msg) if ((sts) != 0) { fprintf(stderr, "pb lib thread %s\tligne %d", msg, __LINE__); exit(EXIT_FAILURE); }

   /**
	* Met en pause le programme et attend une entrée de l'utilisateur
	*/
#define PAUSE(msg) printf("%s [Entrée pour continuer]\n", msg); getchar();

#endif