#include "asset.h"

IMPLEMENT_LIST(Asset_list, Asset *, int, Asset_copy, Int_copy, Asset_cmp, Int_cmp, Asset_free, Int_free);

Asset_list * Asset_load_config()
{
	#ifdef __DEBUG_ASSET_LOAD__
	fprintf(stdout, "\nAsset_load_config()\n");
	#endif
	FILE   * configFile  = NULL;
	char   * buffer      = malloc(sizeof(char) * 255);
	char   * orig_buffer = buffer;
	char   * tileName    = NULL;
	char   * tileSize    = NULL;
	char   * posX        = NULL;
	char   * posY        = NULL;
	int      lineLen     = 0;
	Asset  * asset       = NULL;
	Tile   * tile        = NULL;
	Asset_list * assets = NULL;

	configFile = fopen(ASSETS_CONFIG_FILE, "r");

	if(configFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", ASSETS_CONFIG_FILE);
		perror(NULL);
	}
	else
	{
		assets = Asset_list_new();
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
					asset = Asset_list_add(assets, assets->size + 1, Asset_load(buffer))->value;
					#ifdef __DEBUG_ASSET_LOAD__
					fprintf(stdout, "Asset_load_config: [%s] => ok\n", asset->name);
					#endif
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
					tile = Tile_list_add(asset->tiles, asset->tiles->size + 1, Tile_new(tileName))->value;

					#ifdef __DEBUG_ASSET_LOAD__
					fprintf(stdout, "Asset_load_config: %s.%s => load... ", asset->name, tile->name);
					#endif

					tile->posOnAsset.x = atoi(posX);
					tile->posOnAsset.y = atoi(posY);

					Tile_load(asset, tile->name);

					#ifdef __DEBUG_ASSET_LOAD__
					if(tile->loaded)
						fprintf(stdout, "ok\n");
					else
						fprintf(stdout, "error\n");
					#endif
				}
			}
		}
	}

	free(orig_buffer);
	fclose(configFile);

	return assets;
}

Tile * Asset_getTile(Asset * asset, char * name)
{
	Tile_list_elem_t * it;
	for(it = asset->tiles->array ; it != NULL ; it = it->next)
	{
		if(strcmp(it->value->name, name) == 0)
			return it->value;
	}
	return NULL;
}

Tile * Asset_get(Asset_list * assets, char * path)
{
	char * assetName;
	char * tileName;
	Asset_list_elem_t * it;

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
	asset->tiles    = Tile_list_new();
	asset->name     = malloc(sizeof(char) * strlen(name));
	strcpy(asset->name, name);

	// The 6 is for .png, /, \0 added in path
	path = malloc(sizeof(char) * (strlen(asset->name) + 6 + ASSETS_DIR_LEN));
	sprintf(path, "%s/%s.png", ASSETS_DIR, asset->name);

	asset->surface = IMG_Load(path);
	if(asset->surface == NULL)
	{
		fprintf(stderr, "Error: loading asset '%s' : can't load file %s ; %s\n", name, path, IMG_GetError());
		return NULL;
	}
	SDL_SetColorKey(asset->surface, SDL_TRUE, SDL_MapRGB(asset->surface->format, 0, 0, 0));

	free(path);
	return asset;
}

void Asset_free(Asset * asset)
{
	if(asset != NULL)
	{
		free(asset->name);

		if(asset->surface != NULL)
			SDL_FreeSurface(asset->surface);

		Tile_list_free(asset->tiles);

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
