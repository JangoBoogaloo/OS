CC = gcc
CFLAGS = -Wall -g 
HDRS = protocol.h
SERVER_OBJS = prime_server.o protocol.o
CLIENT_OBJS = prime_client.o protocol.o

all: server client

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o ./server 

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o ./client

prime_server.o: prime_server.c $(HDRS)
	$(CC) $(CFLAGS) -c prime_server.c -o prime_server.o
 
prime_client.o: prime_client.c $(HDRS)
	$(CC) $(CFLAGS) -c prime_client.c -o prime_client.o

protocol.o: protocol.c $(HDRS)
	$(CC) $(CFLAGS) -c protocol.c -o protocol.o

clean: 
	rm -f *.o *~ client *~ server 
