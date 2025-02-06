#include "struct.h"
#include "enhanceTerminal.h"
#include "affichage.h"
#include "serialize.h"
#include "game_requests.h"
#include "game.h"
#include "generateCards.h"

int jouerPartieServeur(Partie partie, socket_t* sockets);
int jouerPartieClient(Partie partie,int idJoueur, socket_t socketHost);