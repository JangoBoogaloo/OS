/*
   a2-skeleton.c
   COMP 3430 Operating Systems

   A version of the pbuf_indexroducer-consumer problem.

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
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_THREADS 10
#define BUF_SIZE 10
#define INPUT_SIZE 81
#define DELIMS " \t\n"
#define EXIT_MSG "<QUIT>"

static char *buffers[BUF_SIZE] = {NULL};
static int consume_index = 0;

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

static pthread_mutex_t buffer_mutex;
static pthread_cond_t cond_empty;
static pthread_cond_t cond_full;
static int stored_count = 0;

void *consume(void *arg)
{
	bool isFinished = false;
	while(!isFinished)
	{
		char *buffer_copy = NULL;

		pthread_mutex_lock(&buffer_mutex);

		while(stored_count ==0)
		{
			printf("consume wait\n");
			pthread_cond_wait(&cond_empty, &buffer_mutex);
		}
		stored_count--;
		pthread_cond_signal(&cond_full);

		buffer_copy = strdup(buffers[consume_index]);
		printf("C: %d-%s\n", consume_index, buffer_copy);
		free(buffers[consume_index]);
		buffers[consume_index] = NULL;
		consume_index++;
		consume_index %= BUF_SIZE;
		pthread_mutex_unlock(&buffer_mutex);
		if(0==strcmp(buffer_copy, EXIT_MSG))
		{
			isFinished = true;
		}
		free(buffer_copy);
		buffer_copy = NULL;
	}
	pthread_exit(NULL);
} 

void produce(const char *buffer, int buf_index)
{
		pthread_mutex_lock(&buffer_mutex);
		while(stored_count == BUF_SIZE)
		{
			printf("produce wait\n");
			pthread_cond_wait(&cond_full, &buffer_mutex);
		}

		buffers[buf_index] = strdup(buffer);
		stored_count++;
		printf("P-%d: %s\n", buf_index, buffers[buf_index]);

		pthread_cond_signal(&cond_empty);

		pthread_mutex_unlock(&buffer_mutex);
}

int main (int argc, char * argv[])
{
	pthread_t consumer_threads[MAX_THREADS] = {0};
	
	pthread_cond_init(&cond_empty, NULL);
	pthread_cond_init(&cond_full, NULL);

	char line[INPUT_SIZE] = {'\0'};
	int num_threads = -1;
	int rc = -1;
	int t = -1;
	int tnum = -1;
	FILE *inputfile = NULL;
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

	rc = pthread_mutex_init(&buffer_mutex, NULL);
	if(0 != rc) printf("Fail to Destroy Mutex\n");

	int i=0;
	for(i=0; i<MAX_THREADS; i++)
	{
		pthread_create(&consumer_threads[i], NULL, consume, NULL);
	}


	bool finish_loading = false;
	int exit_count = 0;
	i=0;
	while(BUF_SIZE != exit_count)
	{
		if(finish_loading)
		{
			produce(EXIT_MSG, i);
			++exit_count;
		}
		else if(!fgets(line, sizeof(line), inputfile))
		{
			finish_loading = true;
			printf("Done loading\n");
			continue;
		}
		else
		{
			produce(line, i);
			memset(line, 0, sizeof(line));
		}

		i++;
		i %= BUF_SIZE;
	}

	for(i=0; i<MAX_THREADS; i++)
	{
		rc = pthread_join(consumer_threads[i], NULL);
		if(0 != rc) printf("Fail to join consumer thread\n");
	}

	pthread_cond_destroy(&cond_empty);
	pthread_cond_destroy(&cond_full);
	pthread_mutex_destroy(&buffer_mutex);
	fclose(inputfile);

	printf("End of processing\n");
	return 0;
}
