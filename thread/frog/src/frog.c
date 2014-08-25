#include <stdbool.h>

#include "frog.h"
#include "console.h"

/* when declare, it is already initialized */
pthread_mutex_t frog_mutex;

const char * const FROG[FROG_HEIGHT] = 
{
	"@@",
	"><"
};

const char * const FROG_CLOSE[FROG_HEIGHT] = 
{
	"^^",
	"=="
};

static bool in_bound(const int x, const int y)
{
	if(x <= SCR_RIGHT && x >= SCR_LEFT 
		 && y <= (SCR_BOTTOM-3) && y >= (SCR_TOP+2))
	{
		syslog(LOG_WARNING, "x %d - y %d", x, y);
		return true;
	}
	return false;
}

void blink_frog(struct frog_t *frog, const int flash_wait)
{
	/* draw frog close eye */
	pthread_mutex_lock(&frog_mutex);
	pthread_mutex_lock(&console_mutex);
	screen_clear_image(frog->y, frog->x, 
									 	 FROG_WIDTH, FROG_HEIGHT);
	screen_draw_image(frog->y, frog->x, 
										(char**)FROG_CLOSE, FROG_HEIGHT);
	pthread_mutex_unlock(&console_mutex);
	pthread_mutex_unlock(&frog_mutex);
	sleep_ticks(flash_wait);

	/* draw frog open eye */
	pthread_mutex_lock(&frog_mutex); 
	pthread_mutex_lock(&console_mutex);
	screen_clear_image(frog->y, frog->x, 
									 	 FROG_WIDTH, FROG_HEIGHT);
	screen_draw_image(frog->y, frog->x, 
										(char**)FROG, FROG_HEIGHT);
	pthread_mutex_unlock(&console_mutex);
	pthread_mutex_unlock(&frog_mutex);
	sleep_ticks(flash_wait);
}

static void draw_frog(struct frog_t prev_frog, struct frog_t *frog)
{
	pthread_mutex_lock(&console_mutex);
	screen_clear_image(prev_frog.y, prev_frog.x, 
									 	 FROG_WIDTH, FROG_HEIGHT);
	screen_draw_image(frog->y, frog->x, 
										(char**)FROG, FROG_HEIGHT);
	pthread_mutex_unlock(&console_mutex);
}

void init_frog(struct frog_t *frog)
{
	pthread_mutex_lock(&frog_mutex);
  draw_frog(*frog, frog);
	pthread_mutex_unlock(&frog_mutex);
}

void move_frog(struct frog_t *frog, const char dir, bool is_wood)
{
	int frog_x = -1;
	int frog_y = -1;
	bool move_frog = false;
	struct frog_t prev_frog = {0};

	pthread_mutex_lock(&frog_mutex);
	prev_frog = *frog;
	frog_x = frog->x;
	frog_y = frog->y;
	move_frog = true;
	switch(dir)
	{
		case 'h':
			frog_x -= 1;
			break;
		case 'l':
			frog_x += 1;
			break;
		case 'j':
			frog_y += 2;
			break;
		case 'k':
			frog_y -= 2;
			break;
		default:
			move_frog = false;
			break;
	}

	if(move_frog && in_bound(frog_x, frog_y))
	{
		frog->x = frog_x;
		frog->y = frog_y;
		if(!is_wood)
		{
			draw_frog(prev_frog, frog);
		}
		else
		{
			draw_frog(*frog, frog);
		}
	}

	pthread_mutex_unlock(&frog_mutex);
}
