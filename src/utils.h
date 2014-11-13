#ifndef __UTILS_H__
#define __UTILS_H__

#include <SDL2/SDL.h>
#include "array.h"

typedef enum _bool
{
	false = 0,
	true
} bool;

typedef struct _Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} Color;

void pause();

#endif // __UTILS_H__