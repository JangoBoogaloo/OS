#include <stdbool.h>

#include "frog.h"
#include "console.h"

const char * const FROG[FROG_HEIGHT] = 
{
	"@@",
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

static void draw_frog(struct frog_t prev_frog, struct frog_t frog)
{
	pthread_mutex_lock(&console_mutex);
	screen_clear_image(prev_frog.y, prev_frog.x, 
									 	 FROG_WIDTH, FROG_HEIGHT);
	screen_draw_image(frog.y, frog.x, 
										(char**)FROG, FROG_HEIGHT);
	pthread_mutex_unlock(&console_mutex);
}

void init_frog(struct frog_t frog)
{
  draw_frog(frog, frog);
}

void move_frog(struct frog_t *frog, const char dir)
{
	struct frog_t prev_frog = *frog;
	int frog_x = frog->x;
	int frog_y = frog->y;
	bool move_frog = true;
	switch(dir)
	{
		case 'h':
			frog_x -= 1;
			break;
		case 'l':
			frog_x += 1;
			break;
		case 'j':
			frog_y += 1;
			break;
		case 'k':
			frog_y -= 1;
			break;
		default:
			move_frog = false;
			break;
	}

	if(move_frog && in_bound(frog_x, frog_y))
	{
		frog->x = frog_x;
		frog->y = frog_y;
		draw_frog(prev_frog, *frog);
	}
}
