#include "tile.h"


IMPLEMENT_LIST(Tile_list,  Tile *,  int, Tile_copy,  Int_copy, Tile_cmp,  Int_cmp, Tile_free,  Int_free);

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
	Tile * tile = Asset_getTile(asset, name);

	if(tile->surface == NULL)
		Graphics_createTileSurface(asset, tile);

	tile->loaded = true;
	return tile;
}

TileShape TileShape_FromStr(char * shapeStr)
{
	if(shapeStr == NULL)
		return TS_FULL;
	trim(&shapeStr);

	if(strcmp(shapeStr, "FULL")              == 0) return TS_FULL;
	if(strcmp(shapeStr, "TRIANGLE_LEFT")     == 0) return TS_TRIANGLE_LEFT;
	if(strcmp(shapeStr, "TRIANGLE_RIGHT")    == 0) return TS_TRIANGLE_RIGHT;
	if(strcmp(shapeStr, "MID_LEFT")          == 0) return TS_MID_LEFT;
	if(strcmp(shapeStr, "MID_RIGHT")         == 0) return TS_MID_RIGHT;
	if(strcmp(shapeStr, "PLOT_TOP_LEFT")     == 0) return TS_PLOT_TOP_LEFT;
	if(strcmp(shapeStr, "PLOT_TOP_RIGHT")    == 0) return TS_PLOT_TOP_RIGHT;
	if(strcmp(shapeStr, "PLOT_BOTTOM_LEFT")  == 0) return TS_PLOT_BOTTOM_LEFT;
	if(strcmp(shapeStr, "PLOT_BOTTOM_RIGHT") == 0) return TS_PLOT_BOTTOM_RIGHT;
	return TS_FULL;
}

char * TileShape_ToStr(TileShape shape)
{
	static char shapeStr[][21] = {
		"TS_FULL             ",
		"TS_TRIANGLE_LEFT    ",
		"TS_TRIANGLE_RIGHT   ",
		"TS_MID_LEFT         ",
		"TS_MID_RIGHT        ",
		"TS_PLOT_TOP_LEFT    ",
		"TS_PLOT_TOP_RIGHT   ",
		"TS_PLOT_BOTTOM_LEFT ",
		"TS_PLOT_BOTTOM_RIGHT"
	};

	return shapeStr[shape];
}