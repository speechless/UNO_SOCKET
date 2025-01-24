all : bin/serveurUNO

bin/serveurUNO : src/serveurUNO.c includes/inc.h libs/libInet.a
	gcc src/serveurUNO.c -Llibs -lInet -Iincludes -o bin/serveurUNO

clean:
	rm bin/*