#ifndef REQUETES
#define REQUETES


#define MAX_BUFF 1024
char buffer[MAX_BUFF];

typedef struct {
	int code;
	char data[MAX_BUFF];
} basic_data_t;

typedef struct {
	int id;
	char adresseHost[20];
	short portHost;
	int nbJoueursMax;
} salon_t;

#endif