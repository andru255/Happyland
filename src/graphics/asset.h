
#ifndef __ASSET_H__
#define __ASSET_H__

#include <stdlib.h>
#include <SDL2/SDL.h>

#include "../string-tools.h"
#include "../config.h"

#include "structures.h"
#include "tile.h"

/**
 * Load assets from the config file
 * @return Array of asset
 */
Asset_array * Asset_load_config();

/**
 * Get a TileConfig struct
 * @param  asset Asset where to search the TileConfig
 * @param  name  Name of the tile
 * @return       A pointer to TileConfig struct, NULL if not found.
 */
Tile * Asset_getTile(Asset * asset, char * name);

/**
 * Load asset from file
 * @param  name Name of the asset to load
 * @return
 */
Asset * Asset_load(char * name);

/**
 * Free asset memory
 * @param asset Asset to free
 */
void Asset_free(Asset * asset);

/**
 * [Asset_copy description]
 * @param dest [description]
 * @param src  [description]
 */
void Asset_copy(Asset ** dest, Asset ** src);

/**
 * [Asset_cmp description]
 * @param  val1 [description]
 * @param  val2 [description]
 * @return      [description]
 */
int Asset_cmp(Asset * val1, Asset * val2);

/**
 * [Asset_get description]
 * @param  assets [description]
 * @param  path   [description]
 * @return        [description]
 */
Tile * Asset_get(Asset_array * assets, char * path);


#endif // __ASSET_H__