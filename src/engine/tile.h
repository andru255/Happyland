#ifndef __TILE_H__
#define __TILE_H__

#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../error-codes.h"
#include "../utils.h"

#include "structures.h"
#include "asset.h"


/**
 * Create a new tile struct
 * @param  name Name of the tile
 * @return      New tile object
 */
Tile * Tile_new(char * name);

/**
 * Load a tile from an asset
 * @param  assetName Asset to use to load the tile
 * @param  position  Name of the tile to load
 * @return           The loaded tile
 */
Tile * Tile_load(Asset * asset, char * name);

/**
 * Free tile memory
 * @param tile Tile to free
 */
void Tile_free(Tile * tile);

/**
 * [Tile_copy description]
 * @param dest [description]
 * @param src  [description]
 */
void Tile_copy(Tile ** dest, Tile ** src);

/**
 * [Tile_cmp description]
 * @param  val1 [description]
 * @param  val2 [description]
 * @return      [description]
 */
int Tile_cmp(Tile * val1, Tile * val2);

#endif // __TILE_H__