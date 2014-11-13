#ifndef __GAME_H__
#define __GAME_H__

#include "graphics/graphics.h"

typedef struct _GameData
{
	// Assets
	Asset_array * assets;

	// Maps
	Map ** maps;
	int    nbrMaps;

} GameData;

#endif // __GAME_H__