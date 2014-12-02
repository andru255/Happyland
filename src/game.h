#ifndef __GAME_H__
#define __GAME_H__

#include "graphics/graphics.h"

#include "characters/player.h"

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

void GameData_free(GameData * gd);

void Game_LoadMap(GameData * gd, char * map);

#endif // __GAME_H__