/**********************************************************************
  Module: client.c
  Author: Jim Young
  Date:   2013 Jan 3

  Purpose: To implement a prime generator to connect to a server to receive
 				work: prime numbers to check.

  Changes: 
    2013 Jan 3 [ Jim Young ]
      - initial version

**********************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "globals.h"
#include "protocol.h"

static int clientID = -1; /* my ID as read in from command line. 0..9 once parsed */
static char myfilename[MAX_FILE_SIZE];	/* to hold our FIFO filename */

/* checks if number is prime. VERY inefficient, because this is a make-work
   task for the CPU */
boolean isPrime(int number) {
	boolean prime = true;
	int factors = 0;
	int i;
	for (i = 1; i <= number; i++) 
		if (number%i == 0)
			factors++;
	prime = factors==2;
	return prime;
}

/* exit method. removes our fifo */
void cleanup() {
   printf("shutting down\n");
	unlink(myfilename);
	exit(0);
}

/* signal handler. Grabs SIGINT (ctrl-c), SIGTERM (kill), and 
    SIGPIPE (strange pipe error - SHOULDNT HAPPEN - and cleans up. */
void ctrl_c(int sig) {
  if (sig == SIGINT || sig == SIGTERM || sig == SIGPIPE) {
   printf("signal received\n");
	cleanup();
 }
}

/* sets up our signal handlers and FIFO */
void setup() {

	signal(SIGINT, ctrl_c);
	signal(SIGTERM, ctrl_c);
	signal(SIGPIPE, ctrl_c);

	sprintf(myfilename, "%s%d", CLIENT_FIFO, clientID);
	mkfifo(myfilename, 0666);
}

void runClient() {
	int myFile;
	int data; /* one byte at a time */
	int running = true;
	int serverFile;
	char const * outmessage;
	ProtocolData packet;

	printf("started client %d\n", clientID);

	/* should do proper error checking on ALL system calls, to provide a graceful way out. */
	while(running) {  /* always true..*/

		// setup and send request message
		outmessage = createMessage(REQUEST, 0, clientID);	
		serverFile = open(SERVER_FIFO, O_WRONLY);
		write(serverFile, outmessage, strlen(outmessage));
		close(serverFile); /* be polite - close FIFO when not in use */

		initProtocol(&packet); /* start with fresh packet */
		myFile = open(myfilename, O_RDONLY);
		while (!packet.complete) { /* read ONE CHAR at a time, to help our packet state machine */
			read(myFile, &data, 1);
			processProtocol(data, &packet);
		}
		close(myFile);

		if (isPrime(packet.data)) { /* if prime, send a response back to the server */
			outmessage = createMessage(PRIME_FOUND, packet.data, clientID);
			serverFile = open(SERVER_FIFO, O_WRONLY);
			write(serverFile, outmessage, strlen(outmessage));
			close(serverFile); /* keep closed */
		}
	}
}


int main(int argc, char** argv) {
	char opt;

	/* Check input parameters */
	while ((opt = getopt(argc, argv, "c:")) != -1)
		switch (opt) {
			case 'c':
				clientID = atoi(optarg);
		}

	if (clientID == -1) {
		fprintf(stderr, "Usage: %s -c clientNo\n", argv[0]);
		exit(1);
	}

	if (clientID >= MAX_CLIENTS) {
		fprintf(stderr, "Max clients reached.\n");
		exit(1);
	}
	
	/* Server running? */
	if (access(SERVER_FIFO, F_OK) == -1) { /* FIFO doesnt exist, no server */
		fprintf(stderr, "No server found.\n");
		exit(1);
	}

	setup();
	runClient(); /* never returns */
	exit(0);
}
