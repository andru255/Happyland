#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "../utils.h"
#include "../c-containers/list.h"

/**
 * This struct contain information
 * about a tile
 */
typedef struct _Tile
{
	char        * name;        ///< Tile Name
	SDL_Point     posOnScreen; ///< Position of the tile on the screen
	SDL_Point     posOnAsset;  ///< Position of the tile on the asset
	bool          loaded;      ///< Is the surface loaded ?
	SDL_Surface * surface;     ///< SDL Surface
} Tile;

NEW_LIST_DEFINITION(Tile_list,  Tile *,  int);


typedef struct _Asset
{
	char        * name;     ///< Asset Name
	SDL_Surface * surface;  ///< SDL_Surface
	Tile_list   * tiles;    ///< Tiles of the asset
} Asset;

NEW_LIST_DEFINITION(Asset_list, Asset *, int);

#endif // __STRUCTURES_H__