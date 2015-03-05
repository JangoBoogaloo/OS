/*
   a2-skeleton.c
   COMP 3430 Operating Systems

   A version of the producer-consumer problem.

   We are using pthread condition variables to emulate a monitor-based
   solution. One thing this solution does NOT do is guarantee that
   when a process signals a condition, another process waiting for
   that signal will immediately start executing. Thus we likely will
   have several processes receiving a signal, but not executing
   immediately. This means that, unlike a monitor-based solution, a
   process should check the condition that caused it to wait (i.e. in
   a while loop) before resuming execution.

   To compile: gcc -lpthread a2-skeleton.c

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define MAX_THREADS 10
#define BUF_SIZE 10
#define INPUT_SIZE 81
#define DELIMS " \t\n"


typedef unsigned long ulong;

// monitor functions for buffer manipulation

/*
   append - monitor function for the producer

   Since there is only 1 producer, we can test
   the blocking condition in an if statement.
*/

/*
   take - monitor function for the consumers

   Because there are multiple consumers, we need to check the blocking
   condition in a while loop, since the condition may be changed
   between the time a consumer receives a signal and the time it
   actually executes.
*/


void *consumer_thread(void *arg)
{

   pthread_exit(NULL);
} // end consumer_thread

int main (int argc, char * argv[])
{
   pthread_t tids[MAX_THREADS];
   char line[INPUT_SIZE];
   int num_threads;
	 int rc;
	 int t;
	 int tnum;
   FILE *inputfile;
   ulong word_total = 0;
   ulong char_total = 0;

   // argument checking
   if(argc < 3)
	 {
      printf("Usage: %s num_threads <input file>\n", argv[0]);
      exit(0);
   }
	 else
	 {
      num_threads = atoi(argv[1]);
	 }

   // open the input file
   inputfile = fopen(argv[2], "r");
   if(inputfile == NULL)
	 {
      printf("Unable to open %s\n", argv[2]);
      exit(0);
   }

   fclose(inputfile);

   printf("End of processing\n");
   pthread_exit(NULL);
} // end main
