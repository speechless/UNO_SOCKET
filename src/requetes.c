#include <requetes.h>
#include <stdio.h>

void serialiserData(basic_data_t* data, char* chaine) {
	sprintf(chaine, "[%d] : %s", data->code, data->data);
	printf("envoi : #%s#\n", (char*)chaine);
}

void deserialiserData(char* chaine, basic_data_t* data) {
	printf("Reçu : #%s#\n", (char*)chaine);
	sscanf(chaine, "[%d] : %[\001-\377]", &data->code, data->data);
	printf("Désérialisé : #[%d] : %s#\n", data->code, data->data);
}
