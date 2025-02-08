# $^ = toutes les dépendances
# $< = la première dépendance
# $@ = la cible

# Nom des exécutables
TARGETS = serveurUNO clientUNO

# Répertoires
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = includes
LIB_DIR = libs

# Options de compilation
CC = gcc
CFLAGS = -Wall -I$(INC_DIR) -L$(LIB_DIR)
LIBS = -lPSY -lInet

# Dépendances globales
DEPS = $(INC_DIR)/inc.h

DEBUG =

# Fichiers sources spécifiques aux cibles
SRC_serveurUNO = serveurUNO.c requetes.c liste.c es.c common.c
SRC_clientUNO = clientUNO.c requetes.c game.c affichage.c serialize.c game_requests.c player.c generateCards.c common.c

# Générer les fichiers objets correspondants à chaque exécutable
OBJ_serveurUNO = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_serveurUNO))
OBJ_clientUNO = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_clientUNO))

# Règles pour la compilation séparée
all: $(addprefix $(BIN_DIR)/, $(TARGETS))

# Génération des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< $(CFLAGS) $(DEBUG) -o $@

# Génération des fichier exécutables
.SECONDEXPANSION:
$(BIN_DIR)/%: $$(OBJ_$$*) 
	@mkdir -p $(BIN_DIR)
	$(CC) $^ $(CFLAGS) $(DEBUG) $(LIBS) -o $@

debug:
	$(MAKE) DEBUG=-DDEBUG


# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*

# Cibles phony
.PHONY: all clean
