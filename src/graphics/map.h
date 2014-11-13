#ifndef __MAP_H__
#define __MAP_H__

#include "../utils.h"

#include "tile.h"
#include "asset.h"

/**
 * This struct contain information
 * about a map
 */
typedef struct _Map
{
	Tile        *** tiles;  ///< Tiles of the map
	SDL_Rect        clip;   ///< Rect displayed on the screen
	char        *   name;   ///< Name of the map
	SDL_Surface *   surface; ///< SDL Surface

	int height;
	int width;
	Color background_color;

	Tile_array * tileMap;
} Map;

/*
 * Maps functions
 */

Map * Map_init(char * name);
/**
 * Load a map from a file to memory
 * @param  name Name of the map to load
 * @return      A filled map structure
 */
Map * Map_load(char * name, Asset_array * assets);

/**
 * Move a map on the screen
 * @param  map   The map to move
 * @param  delta Move to execute
 * @return       The map moved
 */
Map * Map_move(Map * map, SDL_Point delta);

/**
 * Render the map in memory
 * @param  map    The map to render
 * @param  rawmap 2-dimensions array containing the map info
 * @return        [description]
 */
bool Map_render(Map * map, long int * rawmap);

/**
 * Free map memory
 * @param map Map to free
 */
void Map_free(Map * map);

#endif // __MAP_H__