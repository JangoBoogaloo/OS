#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "protocol.h"
#include <assert.h>

#define MODE 0622

static int client_id;
static char file_client[32];
void terminate(int sig)
{		
	if(sig == SIGTERM || sig == SIGINT)
	{
		printf("\nExiting......\n");
		unlink(fifo_client);
		exit(1);
	}

}

void setup()
{
	signal(SIGINT, terminate);
	signal(SIGTERM, terminate);
	sprintf(file_client, "%s%d", CLIENT_FIFO, clientid);
	mkfifo(file_client, MODE); 
}

void run_client()
{
	int fifo_client;
	int fifo_server;
	int byte;

	char const * msg;
	Protocol packet;

	while(true)
	{
		out_msg = create_msg(REQUES, 0, client_id);
		fifo_server = open(SERVER_FIFO, O_WRONLY);
		write(fifo_server, out_msg, strlen(msg));
		close(fifo_server);
		
		init_protocol(&packet);
		fifo_client = open(CLIENT_FIFO, O_RDONLY);
		while(!packet.ready)
		{
			read(fifo_client, &byte, 1);
			load_protocol(byte, &packet);
		}
		close(fifo_client);
		if(checkPrime(packet.data))
		{
			out_msg = create_msg(POST, packet.data, client_id);
			fifo_server = open(SERVER_FIFO, O_WRONLY);
			write(fifo_server, out_msg, strlen(msg));
			close(fifo_server);

		}
	}
}

bool checkPrime(int number)
{
	if(number == 1)
		return false;
	for(int i=2; i<number; i++)
	{
		if(number%i ==0)
			return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	int opt;
	p_ptr = malloc(sizeof(Protocol));

	//set signal handler to handle Terminated signal	
	if(!)
		exit(0);

	if(argc <2)
		perror("usage: <pgm> -<client-id>");

	if((opt = getopt(argc, argv, "c:")) == -1)
	{
		perror("Fail to get Option\n");
		exit(0);
	}
		
	switch(opt)
	{
		case 'c':
			client_id = atoi(optarg);
	}		
	if(access(SERVER_FIFO, F_OK)==-1)
	{
		perror("Can not access server\n");	
		exit(0);
	}

	setup();
	run_client();
		
}
