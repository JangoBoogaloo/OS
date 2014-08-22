#define RIVER_TOP_ROW 4
#define WOOD_ROWS 8
#define DISTANCE 48

#define WOOD_RIGHT_MOST (SCR_WIDTH)
#define WOOD_LEFT_MOST (0-WOOD_WIDTH)
#define MAX_WOOD_HEAP_SIZE 3

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

/* local header */
#include "general.h"
#include "console.h"
#include "frog.h"
#include "wood.h"

/* static sig_atomic_t is_running = true; */
static struct frog_t *frog;
static pthread_cond_t row_create_cvs[WOOD_ROWS];
static pthread_mutex_t row_create_mutex[WOOD_ROWS];
static pthread_t row_threads[WOOD_ROWS];
static pthread_mutex_t frog_mutex;

static void cleanup(void)
{
	/*
  int i=0;
	is_running = false;
	for(i=0; i<WOOD_ROWS; i++)
	{	
		pthread_join(row_threads[i], NULL);
    pthread_mutex_destroy(&row_create_mutex[i]);
    pthread_cond_destroy(&row_create_cvs[i]);
	}
	pthread_mutex_destroy(&console_mutex);
	*/
	screen_fini();
}

static void sighandler_generic(int n)
{
  syslog(LOG_WARNING, "PID %d got signal %d, exiting.\n", 
				 getpid(),  n);
  cleanup();
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

static bool in_wood(struct wood_t wood, struct frog_t *frog)
{
  if(frog->x == wood.x && frog->y == wood.y)
  {
    return true;
  }
  return false;
}

static void *move_wood(void *wood_p)
{
  char frog_move;
	/* copy the struct completely, guarantee atomic */
	struct wood_t wood = *(struct wood_t*)wood_p;

  if(wood.direction > 0)
  {
    frog_move = 'l';
  }
  else
  {
    frog_move = 'h';
  }

	do
	{
    /* move the frog first */
    pthread_mutex_lock(&frog_mutex);
    if(in_wood(wood, frog))
    {
      move_frog(frog, frog_move);
    }
    pthread_mutex_unlock(&frog_mutex);

    /* then move the wood */
		pthread_mutex_lock(&console_mutex);
		screen_clear_image(wood.y, wood.x, 
											 WOOD_WIDTH, WOOD_HEIGHT);
		wood.x += wood.direction;
		screen_draw_image(wood.y, wood.x, 
											(char**)WOOD, WOOD_HEIGHT);
		screen_refresh();
		pthread_mutex_unlock(&console_mutex);

		if((SCR_WIDTH/2) == wood.x)
		{
		  pthread_mutex_lock(&row_create_mutex[wood.row]);
			pthread_cond_signal(&row_create_cvs[wood.row]);
		  pthread_mutex_unlock(&row_create_mutex[wood.row]);
		}
		sleep_ticks(DISTANCE - wood.speed);
	}while(WOOD_RIGHT_MOST >= wood.x && WOOD_LEFT_MOST <= wood.x);

	pthread_exit(NULL);
}

static void *run_row(void *wood_p)
{
	/* copy the struct completely, guarantee atomic */
	struct wood_t wood = *(struct wood_t*)wood_p;
	pthread_t *wood_threads[MAX_WOOD_HEAP_SIZE];

  int i = 0;
  for(i=0; i<MAX_WOOD_HEAP_SIZE; i++)
  {
		/* init the thread pointer to NULL */
    wood_threads[i] = NULL;
  }

  i=0;
	do
	{
		/* wait for used thread to finish and free it */
    if(NULL != wood_threads[i])
    {
      if(0 != pthread_join(*wood_threads[i], NULL))
      {
        APP_EXIT(EXIT_FAILURE);
      }
			syslog(LOG_WARNING, "try free\n");
      wood_threads[i] = NULL;
			syslog(LOG_WARNING, "free wood thread %d of row %d\n", 
						 i, wood.row);
    }

		/* malloc the thread again */
		wood_threads[i] = MALLOC(sizeof(pthread_t));
		syslog(LOG_WARNING, 
					 "Malloc wood thread %d of row %d\n", i, wood.row);

		pthread_mutex_lock(&row_create_mutex[wood.row]);
		pthread_create(wood_threads[i], NULL, move_wood,
									 (void *)&wood);

		if(0 != pthread_cond_wait(&row_create_cvs[wood.row], 
										          &row_create_mutex[wood.row]))
    {
      APP_EXIT(EXIT_FAILURE);
    }

		pthread_mutex_unlock(&row_create_mutex[wood.row]);

		/* circulate the heap */
		i = (i+1)%MAX_WOOD_HEAP_SIZE;
	} while(true);

	pthread_exit(NULL);
}

int main(void) 
{
	struct wood_t wood[WOOD_ROWS] = {{0}};
  int direct = LEFT;
	int i = 0;
	int cmd = EOF;
	frog = MALLOC(sizeof(struct frog_t));
	frog->x = SCR_WIDTH/2;
	frog->y= SCR_BOTTOM-3;
	

	setup_signals();

	if (!screen_init(SCR_HEIGHT, SCR_WIDTH)) 
	{
		printf("Screen Fail to Initialize\n");
		APP_EXIT(1);
	}

	pthread_mutex_init(&console_mutex, NULL);
  pthread_mutex_init(&frog_mutex, NULL);

	for(i=0; i<WOOD_ROWS; i++)
	{
		wood[i].y = 4 + WOOD_HEIGHT * i;
		wood[i].speed = 2*(15+(WOOD_ROWS - i));
		wood[i].row = i;
		wood[i].direction = direct;

		/* keep swapping direction */
		direct = 0 - direct;

		if (LEFT == wood[i].direction)
		{
			wood[i].x = WOOD_RIGHT_MOST;
		}
		else
		{
			wood[i].x = WOOD_LEFT_MOST;
		}
    pthread_mutex_init(&row_create_mutex[i], NULL);
		pthread_cond_init(&row_create_cvs[i], NULL);
		pthread_create(&row_threads[i], NULL, 
									 run_row, (void *)&wood[i]);	
	}

  init_frog(*frog);
	do
	{
		cmd = getchar();

    pthread_mutex_lock(&frog_mutex);
		move_frog(frog, (char)cmd);
    pthread_mutex_unlock(&frog_mutex);

		syslog(LOG_WARNING, "Got CMD\n");
	}while('q' != cmd);

	for(i=0; i<WOOD_ROWS; i++)
	{	
		pthread_join(row_threads[i], NULL);
  }

  cleanup();
	pthread_exit(NULL);
}
