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

static char * buffers[BUF_SIZE] = {NULL};
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
static pthread_mutex_t total_chars_mutex;
static pthread_mutex_t total_words_mutex;

static pthread_cond_t cond_empty;
static pthread_cond_t cond_full;

static int stored_count = 0;

static ulong word_total = 0;
static ulong char_total = 0;

static int calculateTotalChars(const char *buffer, const int size)
{
	int i=-1;
	for(i=0; i<size; i++)
	{
		if( '\0' == buffer[i]) break;
	}
	return i+1;
}

static int calculateTotalWords(const char *buffer, const char *deliminate)
{
	int total_word = 0;
	char *brkt = NULL;
	char *word = NULL;

	for(word = strtok_r((char *)buffer, deliminate, &brkt); word; 
			word = strtok_r(NULL, deliminate, &brkt))
	{
		total_word++;
	}

	return total_word;
}

static void *consume()
{
	unsigned int total_chars = -1;
	unsigned int total_wrds = -1;
	bool isFinished = false;
	char *buffer_copy = NULL;
	while(!isFinished)
	{

		pthread_mutex_lock(&buffer_mutex);

		while(stored_count ==0)
		{
			pthread_cond_wait(&cond_empty, &buffer_mutex);
		}

		stored_count--;
		pthread_cond_signal(&cond_full);

		buffer_copy = strdup(buffers[consume_index]);
		free(buffers[consume_index]);
		buffers[consume_index] = NULL;
		consume_index++;
		consume_index %= BUF_SIZE;
		pthread_mutex_unlock(&buffer_mutex);

		if(0 != strcmp(buffer_copy, EXIT_MSG))
		{
			total_chars = calculateTotalChars(buffer_copy, INPUT_SIZE);
			total_wrds = calculateTotalWords(buffer_copy, DELIMS);

			pthread_mutex_lock(&total_chars_mutex);
			char_total += total_chars;
			pthread_mutex_unlock(&total_chars_mutex);

			pthread_mutex_lock(&total_words_mutex);
			word_total += total_wrds;
			pthread_mutex_unlock(&total_words_mutex);
		}
		else
		{
			isFinished = true;
		}

		free(buffer_copy);
		buffer_copy = NULL;
	}
	pthread_exit(NULL);
} 

static void produce(const char *buffer, const int buf_index)
{
		pthread_mutex_lock(&buffer_mutex);
		while(stored_count == BUF_SIZE)
		{
			pthread_cond_wait(&cond_full, &buffer_mutex);
		}
		buffers[buf_index] = strdup(buffer);
		stored_count++;

		pthread_cond_signal(&cond_empty);

		pthread_mutex_unlock(&buffer_mutex);
}

int main (int argc, char * argv[])
{
	pthread_t consumer_threads[MAX_THREADS] = {0};
	char line[INPUT_SIZE] = {'\0'};
	int num_threads = -1;
	FILE *inputfile = NULL;

	// argument checking
	if(argc < 3)
	{
		printf("Usage: %s num_threads <input file>\n", argv[0]);
		exit(0);
	}
	else
	{
		num_threads = atoi(argv[1]);
		if(num_threads > MAX_THREADS)
		{
			printf("Usage: the program can create at most %d threads\n", MAX_THREADS);
			exit(0);
		}

	}

	bool finish_reading = false;
	int thread_exit_count = 0;
	int i=-1;

	printf("start processing\n");
	// open the input file
	inputfile = fopen(argv[2], "r");
	if(inputfile == NULL)
	{
		printf("Unable to open %s\n", argv[2]);
		exit(0);
	}

	/* create condition variable to handle buffer empty and buffer full */
	pthread_cond_init(&cond_empty, NULL);
	pthread_cond_init(&cond_full, NULL);

	pthread_mutex_init(&buffer_mutex, NULL);
	pthread_mutex_init(&total_chars_mutex, NULL);
	pthread_mutex_init(&total_words_mutex, NULL);

	finish_reading = false;
	thread_exit_count = 0;
	for(i=0; i<num_threads; i++)
	{
		pthread_create(&consumer_threads[i], NULL, consume, NULL);
	}

	i=0;
	while(BUF_SIZE != thread_exit_count)
	{
		if(finish_reading)
		{
			/* send exit message to buffer to end thread */
			produce(EXIT_MSG, i);
			++thread_exit_count;
		} 
		else if(!fgets(line, INPUT_SIZE, inputfile))
		{
			/* finished reading file */
			finish_reading = true;
			printf("Done Reading File\n");
			continue;
		}
		else
		{
			/* produce one line of string to buffers */
			produce(line, i);
			/* clean line */
			memset(line, '\0', sizeof(line));
		}

		i++;
		i %= BUF_SIZE;
	}

	for(i=0; i<num_threads; i++)
	{
		pthread_join(consumer_threads[i], NULL);
	}
	pthread_cond_destroy(&cond_empty);
	pthread_cond_destroy(&cond_full);
	pthread_mutex_destroy(&buffer_mutex);
	pthread_mutex_destroy(&total_chars_mutex);
	pthread_mutex_destroy(&total_words_mutex);

	fclose(inputfile);

	printf("Tatal Characters: %lu\n", char_total);
	printf("Tatal Words: %lu\n", word_total);

	char_total = 0;
	word_total = 0;
	stored_count =0;
	consume_index = 0;

	printf("End of processing\n");

	return 0;
}
