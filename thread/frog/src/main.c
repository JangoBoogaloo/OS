#define RIVER_TOP_ROW 4
#define WOOD_ROWS 3

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

void sighandler_generic(int n)
{
  syslog(LOG_WARNING, "PID %d got signal %d, exiting.\n", 
				 getpid(),  n);
	screen_fini();
  exit(-1);
}

void setup_signals()
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

int main(void) 
{
	struct wood_t wood = {0};
	wood.row = 4;
	wood.col = 0;
	wood.speed = 1;
	if (!screen_init(SCR_HEIGHT, SCR_WIDTH)) 
	{
		printf("Screen Fail to Initialize\n");
		APP_EXIT(1);
	}

	do
	{
		screen_clear_image(wood.row, wood.col - WOOD_WIDTH, 
											 WOOD_WIDTH, WOOD_HEIGHT);
		wood.col = (wood.col + wood.speed) 
								% (SCR_WIDTH + WOOD_WIDTH);
		screen_draw_image(wood.row, wood.col - WOOD_WIDTH, 
											(char**)WOOD, WOOD_HEIGHT);
		screen_refresh();
		sleep_ticks(10);
	}while(true);

	return 0;
}
