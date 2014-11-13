#include "tile.h"


IMPLEMENT_ARRAY(Tile_array,  Tile *,  int, Tile_copy,  Int_copy, Tile_cmp,  Int_cmp, Tile_free,  Int_free);

Tile * Tile_new(char * name)
{
	Tile * tile = malloc(sizeof(Tile));
	tile->name = malloc(sizeof(name));
	strcpy(tile->name, name);
	tile->posOnAsset.x  = -1;
	tile->posOnAsset.y  = -1;
	tile->posOnScreen.x = -1;
	tile->posOnScreen.y = -1;
	tile->loaded  = false;
	tile->surface = NULL;
	return tile;
}

void Tile_free(Tile * tile)
{
	if(tile != NULL)
	{
		free(tile->name);

		if(tile->surface != NULL)
			SDL_FreeSurface(tile->surface);

		free(tile);
	}
}

void Tile_copy(Tile ** dest, Tile ** src)
{
	*dest = malloc(sizeof(Tile));
	memcpy(*dest, *src, sizeof(Tile));
}

int Tile_cmp(Tile * val1, Tile * val2)
{
	return memcmp(val1, val2, sizeof(Tile));
}

Tile * Tile_load(Asset * asset, char * name)
{
	SDL_Rect blitRect;
	Tile * tile = Asset_getTile(asset, name);

	if(tile->surface == NULL)
		Graphics_createTileSurface(asset, tile);

	tile->loaded = true;

	return tile;
}
