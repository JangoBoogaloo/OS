/**  
 * A program to create some threads using the pthreads library.
 *
 * To compile: gcc -lpthread threadeg1.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 10

void *firstThread(void *tid)
{
  printf("\nI am thread <%d>, running the code firstThread\n", (int)tid);
	sleep(2);
  printf("\nthread <%d> Exit\n", (int)tid);
  pthread_exit(NULL);
} // end firstThread


void *secondThread(void *tid)
{
  printf("\nI am thread <%d>, running the code secondThread\n", (int)tid);
	sleep(2);
  printf("\nthread <%d> Exit\n", (int)tid);
  pthread_exit(NULL);
} // end secondThread


int main (int argc, char * argv[])
{

  pthread_t threads[NUM_THREADS];
  int rc;
  /*
    The loop index is a long so that the thread functions can use
    it as the function parameter without casting.
  */
  long t;

  for(t=0; t<NUM_THREADS; t++)
	{
    printf("Creating thread number %ld\n", t);
		if(t%2==0)
		{
			rc = pthread_create(&threads[t], NULL, firstThread, (void *)t);
		}
		else
		{
      rc = pthread_create(&threads[t], NULL, secondThread, (void *)t);
		}
		if(rc)
		{
			printf("ERROR - return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
		else
		{
			printf("Successfully created thread with id <%d>\n",(int)threads[t]);
		}
		sleep(1);
  }
	printf("Finsh main loop\n");
  pthread_exit(NULL);
} 
