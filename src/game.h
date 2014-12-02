#ifndef __GAME_H__
#define __GAME_H__

#include "graphics/graphics.h"

typedef struct _GameData
{
	// Assets
	Asset_list * assets;

	// Maps
	Map_list * maps;

	// SDL Window
	SDL_Window  * window;
	SDL_Surface * screen;

} GameData;

GameData * GameData_new();

#endif // __GAME_H__