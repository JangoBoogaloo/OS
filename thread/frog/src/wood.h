#ifndef WOOD_H__
#define WOOD_H__

#define WOOD_WIDTH 15
#define WOOD_HEIGHT 2

#include "general.h"

struct wood_t
{
	int y;
	int x;
	int row;
	int direction;
	int speed;
};

extern const char * const WOOD[WOOD_HEIGHT];
#endif
