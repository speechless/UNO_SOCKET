


# Nom de l'exécutable
TARGET1 = serveurUNO
TARGET2 = clientUNO

# Dépendances target
DEPS = includes/inc.h

# Répertoires
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = includes
LIB_DIR = libs
OBJ_DIR = obj

# Options de compilation
CC = gcc
LIBS = -lPSY -lInet
CFLAGS = -Wall -I$(INC_DIR) -L$(LIB_DIR) $(LIBS)#-g

# Règle par défaut : compile le programme
all: $(BIN_DIR)/$(TARGET1) $(BIN_DIR)/$(TARGET2)

# Règle pour compiler les fichiers .c en fichiers .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c $< $(CFLAGS) -o $@

$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(DEPS)
	$(CC) $< $(CFLAGS) -o $@

# Nettoyage des fichiers générés
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*


# Afficher l'état du Makefile
.PHONY: all clean