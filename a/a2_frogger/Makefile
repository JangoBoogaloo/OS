CC			= gcc
CFLAGS 	= -Wall -g
LIBS		= -lncurses
SRC_DIR	= src
OBJ_DIR	= obj

SRC			= console.c main.c 
OBJ			= console.o main.o
TARGET	= frog

$(TARGET): $(OBJ_DIR)/console.o $(OBJ_DIR)/main.o
	$(CC) -o $@ $^ $(LIBS)

$(OBJ_DIR)/console.o: $(SRC_DIR)/console.c $(SRC_DIR)/console.h
	$(CC) -c $(CFLAGS) -o "$@" "$<"

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c 
	$(CC) -c $(CFLAGS) -o "$@" "$<"

clean:
	rm -f $(OBJ_DIR)/* $(TARGET)
