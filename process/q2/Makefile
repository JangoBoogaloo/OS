CC = clang
CFLAGS = -Wall -g -I.
OBJ_DIR = bin
SRC_DIR = src
CLIENT_SRC = prime_client.c protocol.c
CLIENT_OBJS = $(addprefix $(OBJ_DIR)/, $(CLIENT_SRC:.c=.o))

SERVER_SRC = prime_server.c protocol.c
SERVER_OBJS = $(addprefix $(OBJ_DIR)/, $(SERVER_SRC:.c=.o))

TARGETS = server client
all: $(TARGETS) 

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS)

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS)

run: all
	./test.sh $(TARGETS)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) -o "$@" "$<" 

clean: 
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(TARGETS)
