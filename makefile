SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = inc

C = gcc
CFLAGS = -Wall -Werror -fsanitize=address -fno-omit-frame-pointer -g -I $(INC_DIR)
LDFLAGS = -fsanitize=address -fno-omit-frame-pointer

EXE = $(BIN_DIR)/shell.out

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(EXE)

$(EXE): $(OBJECTS) | $(BIN_DIR)
	$(C) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(C) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)