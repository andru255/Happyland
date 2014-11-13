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
	Tile        *** tiles;   ///< Tiles of the map
	int         **  layers;  ///< Layer identifier for each tile of the map
	SDL_Rect        clip;    ///< Rect displayed on the screen
	char        *   name;    ///< Name of the map
	SDL_Surface *   surface; ///< SDL Surface

	int   height;
	int   width;
	Color background_color;

	Tile_array * tileMap;
} Map;

NEW_ARRAY_DEFINITION(Map_array, Map *, char *);

/**
 * [Map_init description]
 * @param  name [description]
 * @return      [description]
 */
Map * Map_init(char * name);

/**
 * [Map_load description]
 * @param  assets [description]
 * @return        [description]
 */
Map * Map_load(Map * map, Asset_array * assets);

/**
 * Load a map from a file to memory
 * @param  name Name of the map to load
 * @return      A filled map structure
 */
Map * Map_init_load(char * name, Asset_array * assets);

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
bool Map_render(Map * map, long int * rawmap, int * rawmaplayer);

/**
 * Free map memory
 * @param map Map to free
 */
void Map_free(Map * map);

void Map_copy(Map ** dest, Map ** src);
int Map_cmp(Map * val1, Map * val2);

#endif // __MAP_H__