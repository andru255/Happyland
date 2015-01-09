#ifndef __MAP_H__
#define __MAP_H__

#include <assert.h>

#include "../utils.h"
#include "../config.h"
#include "../c-containers/list.h"

#include "structures.h"
#include "tile.h"
#include "asset.h"

/**
 * Load the map list
 * @return The Map List
 */
Map_list * Map_load_list();

/**
 * [Map_init description]
 * @param  name [description]
 * @return      [description]
 */
Map * Map_init(char * name, char * filename);

/**
 * [Map_load description]
 * @param  assets [description]
 * @return        [description]
 */
Map * Map_load(Map * map, Asset_list * assets);

/**
 * Load a map from a file to memory
 * @param  name Name of the map to load
 * @return      A filled map structure
 */
Map * Map_init_load(char * name, char * filename, Asset_list * assets);

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

Layer Map_GetPixelLayer(Map * map, int x, int y);

bool Layer_IsSolid(Layer layer);

void Layer_FillPixels(Layer ** layerMap, int x, int y, Layer value, TileShape shape);

char * Layer_GetName(Layer layer);

void Map_AddLayerOverlay(Map * map, SDL_Surface * dest);

Layer Map_ConvertEdgeToLayer(char * value);

#endif // __MAP_H__