#define PORT_SVC 5000
#define INADDR_SVC "127.0.0.1"

#define MAX_BUFF 1024
char buffer[MAX_BUFF];

typedef struct {
	int codeRequete;
	char requete[MAX_BUFF];
} requete_t;

typedef struct {
	int codeReponse;
	char reponse[MAX_BUFF];
} reponse_t;