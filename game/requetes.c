#include "requetes.h"

void reqEnvoiPartie(/*int* sockets,*/ Partie partie){
    for(int i=0; i<partie.nbJoueurs; i++){
        char* chaine = malloc(1000 * sizeof(char));
        serialiserPartie(partie, chaine);
        //send(sockets[i], chaine, strlen(chaine), 0);
        free(chaine);
    }
}

void resEnvoiPartie(/*int socket,*/ Partie partie){
    char* chaine = malloc(1000 * sizeof(char));
    deserialiserPartie(chaine, &partie);
    free(chaine);
}

void reqEnvoiCoup(/*int* sockets,*/ Partie partie){
    for(int i=0; i<partie.nbJoueurs; i++){
        char* chaine = malloc(1000 * sizeof(char));
        serialiserCoup(partie, chaine);
        //send(sockets[i], chaine, strlen(chaine), 0);
        free(chaine);
    }
}

void resEnvoiCoup(/*int socket,*/ Partie partie){
    char* chaine = malloc(1000 * sizeof(char));
    deserialiserCoup(chaine, &partie);
    free(chaine);
}