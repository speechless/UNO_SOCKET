#ifndef REQUETES
#define REQUETES


#define MAX_BUFF 1024

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

void serialiserData(basic_data_t* data, char* chaine);
void deserialiserData(char* chaine, basic_data_t* data);

#endif