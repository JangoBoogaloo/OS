#ifndef FROG_H__
#define FROG_H__

#define FROG_HEIGHT 2
#define FROG_WIDTH 2

#include "general.h"

struct frog_t
{
	int y;
	int x;
	int row;
	int live;
};

extern void init_frog(struct frog_t frog);

extern void move_frog(struct frog_t *frog, const char dir);
#endif
