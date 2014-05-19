#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include "protocol.h"
#include <sys/wait.h>

#define MODE 0622  //r---w--w-

void handler(int sig)
{

	if(sig == SIGTERM || sig == SIGINT)
	{
		printf("\nExiting......\n");
		unlink(SERVER_FIFO);
		exit(1);
	}

}

void setup()
{
	signal(SIGINT, handler);
	signal(SIGTERM, handler);	
	mkfifo(SERVER_FIFO, MODE); 
}

void run_server()
{
	
}


