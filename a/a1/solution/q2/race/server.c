/**********************************************************************
  Module: server.c
  Author: Jim Young
  Date:   2013 Jan 3

  Purpose: implements the prime server that farms out primes to be checked to clients.

  Changes: 
    2013 Jan 3 [ Jim Young ]
      - initial version

**********************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "globals.h"
#include "protocol.h"

static int largestPrime = 13;  /* start here. changes as we find new ones */
static int nextPrime    = 13;  /* next to check. usually != largestPrime as many checked are not prime */

static int serverFile = -1; /* file des for our incoming FIFO for listening */


/* remove our fifo and close our file. pack up shop*/
void cleanupAndQuit(int code) {
	if (serverFile != -1)
		close(serverFile);
	unlink(SERVER_FIFO);
	exit(code);
}

/* signal handler for SIGINT (ctrl-c) and SIGTERM (kill) */
void ctrl_c(int sig) {
  if (sig == SIGINT || sig == SIGTERM) {
   printf("shutting down\n");
	cleanupAndQuit(0);
  }
}


/* setup signals and FIFO */
void setup() {
	mkfifo(SERVER_FIFO, 0666);

	signal(SIGINT, ctrl_c);
	signal(SIGTERM, ctrl_c);

}

void runServer() {
	ProtocolData packet;
	char const* outmessage;
	char clientFIFOName[MAX_FILE_SIZE];
	int clientFIFO;
	int result = 0;

	printf("Opening server fifo (blocks until first client)...\n");
	serverFile = open(SERVER_FIFO, O_RDONLY); 
	if (serverFile == -1) {
		perror("Error opening server fifo"); /* exists with bad permissions?? */
		cleanupAndQuit(1);
	}
	printf("Server running...\n");

	char data;	

	while (true) {

		initProtocol(&packet); /* fresh packet */
		while (!packet.complete) {  /* read one byte at a time */
			result = read(serverFile, &data, 1);
			if (result == 1) 
				processProtocol(data, &packet);
			else if (result == -1) {
				perror("reading from Server FIFO");
				cleanupAndQuit(1);
			} else if (result == 0) { /* EOF!!! - RESET by opening and closing again */
				int newfile = open(SERVER_FIFO, O_RDONLY);
				close(serverFile);
				serverFile = newfile;
			}
		}
		
		switch (packet.message) { /* what kind of packet was it? */

			case (REQUEST): /* send out a prime */
				outmessage = createMessage(QUERY, nextPrime, SERVER_ID);
				sprintf(clientFIFOName, "%s%d", CLIENT_FIFO, packet.sourceID); /* select client fifo */
				clientFIFO = open(clientFIFOName, O_WRONLY);  /* should block until someone connects to us */
				write(clientFIFO, outmessage, strlen(outmessage));
				close(clientFIFO); /* be polite - close FIFO when not using */
				nextPrime += 2;
			break;

			case (PRIME_FOUND): 
				/* out of order messages means we may get a smaller positive 
				   after a larger one already came in. */
				if (largestPrime < packet.data) {
					largestPrime = packet.data; 
					printf("%d is our largest prime.\n", largestPrime);
				}

				break;
				default:
				; /* do nothing */
		}
	}
}

int main(int argc, char **argv) {
	setup();
	runServer();
	return(0);
}


