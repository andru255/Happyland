#include "asset.h"

IMPLEMENT_ARRAY(Asset_array, Asset *, int, Asset_copy, Int_copy, Asset_cmp, Int_cmp, Asset_free, Int_free);

Asset_array * Asset_load_config()
{
	FILE   * configFile  = NULL;
	char   * buffer      = malloc(sizeof(char) * 255);
	char   * orig_buffer = buffer;
	char   * tileName    = NULL;
	char   * tileSize    = NULL;
	char   * posX        = NULL;
	char   * posY        = NULL;
	int      lineLen     = 0;
	int      nbrAssets   = 0;
	Asset  * asset       = NULL;
	Tile   * tile        = NULL;
	Asset_array * assets = NULL;

	int i;

	configFile = fopen(ASSETS_CONFIG_FILE, "r");

	if(configFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", ASSETS_CONFIG_FILE);
		perror(NULL);
	}
	else
	{
		assets = Asset_array_new();
		while(fgets(buffer, 255, configFile) != NULL)
		{
			trim(&buffer);

			if(buffer[0] != '#' && buffer[0] != '\0')
			{
				// Remove comment at the end of the line
				trimComment(&buffer);
				lineLen = strlen(buffer);

				// New asset
				if(buffer[0] == '[' && buffer[lineLen - 1] == ']')
				{
					// Extract the name
					buffer[lineLen - 1] = '\0';
					buffer = buffer + 1;

					// Allocate memory for the new asset
					asset = Asset_array_add(assets, assets->size + 1, Asset_load(buffer))->value;
				}
				// New tile
				else if(asset != NULL)
				{
					// Extract name
					tileName = buffer;
					tileSize = cut(tileName, '=');
					trim(&tileName);
					trim(&tileSize);

					// Extract position
					posX = tileSize;
					posY = cut(tileSize, ',');
					trim(&posX);
					trim(&posY);

					// Allocate memory for the new tile
					tile = Tile_array_add(asset->tiles, asset->tiles->size + 1, Tile_new(tileName))->value;

					tile->posOnAsset.x = atoi(posX);
					tile->posOnAsset.y = atoi(posY);

					Tile_load(asset, tile->name);
				}
			}
		}
	}

	free(orig_buffer);

	return assets;
}

Tile * Asset_getTile(Asset * asset, char * name)
{
	Tile_array_elem_t * it;
	for(it = asset->tiles->array ; it != NULL ; it = it->next)
	{
		if(strcmp(it->value->name, name) == 0)
			return it->value;
	}
	return NULL;
}

Tile * Asset_get(Asset_array * assets, char * path)
{
	int i;
	char * assetName;
	char * tileName;
	Asset_array_elem_t * it;

	assetName = trimCpy(path);
	tileName  = cut(assetName, '.');

	for(it = assets->array ; it != NULL ; it = it->next)
	{
		if(strcmp(it->value->name, assetName) == 0)
			return Asset_getTile(it->value, tileName);
	}

	return NULL;
}

Asset * Asset_load(char * name)
{
	char  * path;
	Asset * asset = malloc(sizeof(Asset));
	asset->surface  = NULL;
	asset->tiles    = Tile_array_new();
	asset->name     = malloc(sizeof(char) * strlen(name));
	strcpy(asset->name, name);

	// The 6 is for .png, /, \0 added in path
	path = malloc(sizeof(char) * (strlen(asset->name) + 6 + ASSETS_DIR_LEN));
	sprintf(path, "%s/%s.png", ASSETS_DIR, asset->name);

	asset->surface = (SDL_Surface *)IMG_Load(path);
	SDL_SetColorKey(asset->surface, SDL_TRUE, SDL_MapRGB(asset->surface->format, 0, 0, 0));

	free(path);
	return asset;
}

void Asset_free(Asset * asset)
{
	int i;
	if(asset != NULL)
	{
		free(asset->name);

		if(asset->surface != NULL)
			SDL_FreeSurface(asset->surface);

		Tile_array_free(asset->tiles);

		free(asset);
	}
}

void Asset_copy(Asset ** dest, Asset ** src)
{
	if(*src == NULL)
		*dest = NULL;
	else
	{
		*dest = malloc(sizeof(Asset));
		memcpy(*dest, *src, sizeof(Asset));
	}
}

int Asset_cmp(Asset * val1, Asset * val2)
{
	return memcmp(val1, val2, sizeof(Asset));
}
