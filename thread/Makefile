TARGET	= frog_game
HDR		  = console.h
SRC			= console.c main.c 
OBJ			= $(SRC:.c=.o)

CC			= clang
CFLAGS 	= -Wall -g -I/usr/local/include
LDFLAGS =	-lncurses

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HDR)
	$(CC) -c $(CFLAGS) -o "$@" "$<"

clean:
	rm -f $(OBJ) $(TARGET)
