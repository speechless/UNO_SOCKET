#ifndef REQUETES
#define REQUETES

#define MAX_BUFF 1024

#include <session.h>

typedef enum {
	SALON,
	DEMANDE_SALON,
} type_requete;

typedef struct {
	int code;
	char data[MAX_BUFF];
} basic_data_t;

typedef struct {
	int id;
	int isPrivate;
	int isHost;
	char adresseHost[20];
	unsigned short portHost;
	int nbJoueursActuels;
	int nbJoueursMax;
} salon_t;

typedef struct {
	int isPrivate;
	char codeSalon[20];
	int nbJoueursMax;
} demande_salon;



void serialiserData(basic_data_t* data, char* chaine);
void deserialiserData(char* chaine, basic_data_t* data);

void serialiserSalon(salon_t* salon, char* chaine);
void deserialiserSalon(char* chaine, salon_t* salon);
void initSalon(salon_t* salon, int id, int isPrivate, int isHost, char* adresseHost, short portHost, int nbJoueursActuels, int nbJoueursMax) ;
void envoyerSalon(socket_t socket, salon_t salon);
void afficherSalon(salon_t salon);

void serialiserDemandeSalon(demande_salon* demande, char* chaine);
void deserialiserDemandeSalon(char* chaine, demande_salon* demande);
#endif