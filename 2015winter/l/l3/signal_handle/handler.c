/*
  handler.c
  COMP 3430 Operating Systems
  
  This process sets up a signal handler for the SIGUSR1 signal
  
  to compile: gcc handler.c
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static void catch_signal(int the_signal ) {
	printf("First time catch SIGQUIT\n");
	signal(the_signal,SIG_DFL);
} 

int main ( void ) {
  int i ;

  printf("This is process %d looping forever\n", (int)getpid());
  
  if (signal(SIGQUIT,catch_signal)==SIG_ERR) {
    perror("SIGQUIT handler could not be setup");
    exit(1);
  }

  for (i=0;;i++) {
    printf("%d\n",i); 
		sleep(5); //sleep 5 seconds
  }
  
} // end main


