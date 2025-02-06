#include "game_requests.h"

void reqEnvoiPartie(socket_t* sockets, Partie partie){
    for(int i=0; i<partie.nbJoueurs; i++){
        envoyer(sockets[i], &partie,serialiserPartie);
    }
}

void resEnvoiPartie(socket_t socket, Partie partie){
    recevoir(socket, &partie, deserialiserPartie);
}

void reqEnvoiCoup(socket_t* sockets, Partie partie){
    for(int i=0; i<partie.nbJoueurs; i++){
        envoyer(sockets[i], &partie,serialiserCoup);
    }
}

void resEnvoiCoup(socket_t socket, Partie partie){
    recevoir(socket, &partie, deserialiserCoup);
}