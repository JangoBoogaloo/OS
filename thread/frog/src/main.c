#define RIVER_TOP_ROW 4
#define WOOD_HEIGHT 3
#define WOOD_ROWS 5
#define DISTANCE 151 

#define WOOD_RIGHT_MOST (SCR_WIDTH)
#define WOOD_LEFT_MOST (0-WOOD_WIDTH)

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <syslog.h>

/* pthread function */
#include <pthread.h>

/* local header */
#include "general.h"
#include "console.h"
#include "wood.h"

static pthread_mutex_t console_mutex;
static void sighandler_generic(int n)
{
  syslog(LOG_WARNING, "PID %d got signal %d, exiting.\n", 
				 getpid(),  n);
	screen_fini();
  exit(n);
}

static void setup_signals()
{
  signal(SIGINT  , sighandler_generic);
  signal(SIGTERM , sighandler_generic);
  signal(SIGHUP  , sighandler_generic);
  signal(SIGQUIT , sighandler_generic);
  signal(SIGILL  , sighandler_generic);
  signal(SIGABRT , sighandler_generic);
  signal(SIGFPE  , sighandler_generic);
  signal(SIGKILL , sighandler_generic);
  signal(SIGSEGV , sighandler_generic);
  signal(SIGPIPE , SIG_IGN);
}

static void *move_wood(void *wood_p)
{
	struct wood_t wood = *(struct wood_t*)wood_p;

	do
	{
		pthread_mutex_lock(&console_mutex);
		screen_clear_image(wood.row, wood.col, 
											 WOOD_WIDTH, WOOD_HEIGHT);
		wood.col+= wood.direction;
		screen_draw_image(wood.row, wood.col, 
											(char**)WOOD, WOOD_HEIGHT);
		screen_refresh();
		pthread_mutex_unlock(&console_mutex);
		sleep_ticks(DISTANCE/wood.speed);
	}
	while(WOOD_RIGHT_MOST >= wood.col && WOOD_LEFT_MOST <= wood.col);

	pthread_exit(NULL);
}

int main(void) 
{
	pthread_t wood_thread[WOOD_ROWS];
	struct wood_t wood[WOOD_ROWS];
	int direct = LEFT;

	setup_signals();

	if (!screen_init(SCR_HEIGHT, SCR_WIDTH)) 
	{
		printf("Screen Fail to Initialize\n");
		APP_EXIT(1);
	}

	pthread_mutex_init(&console_mutex, NULL);

	for(int i=0; i<WOOD_ROWS; i++)
	{
		wood[i].row = 4 + 3 * i;
		wood[i].speed = 2*(6+(WOOD_ROWS - i));
		wood[i].direction = direct;

		//keep swapping direction 
		direct = 0 - direct;

		if (LEFT == wood[i].direction)
		{
			wood[i].col = WOOD_RIGHT_MOST;
		}
		else
		{
			wood[i].col = WOOD_LEFT_MOST;
		}
		pthread_create(&wood_thread[i], NULL, 
									 move_wood, (void *)&wood[i]);
	}

	for(int i=0; i<WOOD_ROWS; i++)
	{	
		pthread_join(wood_thread[i], NULL);
	}

	pthread_mutex_destroy(&console_mutex);
	screen_fini();
	syslog(LOG_WARNING, "End of Processing.\n");
	pthread_exit(NULL);
}
