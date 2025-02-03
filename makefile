# Nom des exécutables
TARGETS = serveurUNO clientUNO hostingClientUNO

TARGETS_PATH = $(patsubst %, $(BIN_DIR)/%, $(TARGETS))

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

# Fichiers sources spécifiques aux cibles
SRC_serveurUNO = serveurUNO.c requetes.c liste.c es.c
SRC_clientUNO = clientUNO.c requetes.c
SRC_hostingClientUNO = hostingClient.c requetes.c

# Règles pour la compilation séparée
all: $(TARGETS_PATH)

$(BIN_DIR)/serveurUNO: $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_serveurUNO))
	@mkdir -p $(BIN_DIR)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $(BIN_DIR)/serveurUNO

$(BIN_DIR)/clientUNO: $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_clientUNO))
	@mkdir -p $(BIN_DIR)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $(BIN_DIR)/clientUNO

$(BIN_DIR)/hostingClientUNO: $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_hostingClientUNO))
	@mkdir -p $(BIN_DIR)
	$(CC) $^ $(CFLAGS) $(LIBS) -o $(BIN_DIR)/hostingClientUNO

# Génération des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJ_DIR)/* $(BIN_DIR)/*

# Cibles phony
.PHONY: all clean
