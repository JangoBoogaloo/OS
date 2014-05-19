CC = gcc
CFLAGS = -Wall -g

all: server client

server: server.o protocol.o
	$(CC) $(CFLAGS) server.o protocol.o -o server

client: client.o protocol.o
	$(CC) $(CFLAGS) client.o protocol.o -o client

client.o: client.c
	$(CC) $(CFLAGS) -c client.c 

server.o: server.c protocol.h protocol.c
	$(CC) $(CFLAGS) -c server.c

protocol.o: protocol.c protocol.h
	$(CC) $(CFLAGS) -c protocol.c 
  
clean:
	rm -f server.o protocol.o *~ server client

