#ifndef PIXEL_H
#define PIXEL_H

#include "types.h"

typedef struct pixel pixel;
#ifdef _WIN32
struct pixel
{
	byte b;
	byte g;
	byte r;
	byte padding;
};
#endif

#endif