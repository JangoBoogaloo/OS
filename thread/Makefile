TARGET	= frog_game
SRC			= console.c main.c 
OBJ			= $(SRC:.c=.o)

CC			= clang
CFLAGS 	= -Wall -g -I/usr/local/include -I.
LDFLAGS =	-lncurses

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c 
	$(CC) -c $(CFLAGS) -o "$@" "$<"

clean:
	rm -f $(OBJ) $(TARGET)
