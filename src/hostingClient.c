#include <requetes.h>
#include <stdio.h>
#include <data.h>

int main()
{
    salon_t salon;
    socket_t se;
    initSalon(&salon,0,0,0,"127.0.0.1",5000,1,3);
    se = creerSocketEcoute("127.0.0.1", 5000);
    afficherSalon(salon);

    return 0;
}

