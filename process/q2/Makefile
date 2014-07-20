CC = clang
CFLAGS = -Wall -g -I.
CLIENT_SRC = prime_client.c protocol.c
CLIENT_OBJS = $(CLIENT_SRC:.c=.o)

SERVER_SRC = prime_server.c protocol.c
SERVER_OBJS = $(SERVER_SRC:.c=.o)
TARGETS = server client
all: $(TARGETS) 

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o "$@" "$<" 

clean: 
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(TARGETS)
