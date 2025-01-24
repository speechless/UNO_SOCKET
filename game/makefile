# Nom de l'exécutable final
EXEC = generateCards

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11 -O2

# Fichiers source
SRCS = generateCards.c

# Fichiers objets générés
OBJS = $(SRCS:.c=.o)

# Règle principale (par défaut)
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers objets
%.o: %.c enhanceTerminal.h struct.h
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJS) $(EXEC)

# Phony pour éviter les conflits avec des fichiers portant les mêmes noms
.PHONY: all clean
