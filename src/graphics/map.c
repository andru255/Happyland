#include "map.h"

Map * Map_init(char * name)
{
	Map * tmp     = malloc(sizeof(Map));
	tmp->tiles    = NULL;
	tmp->name     = malloc(sizeof(char) * strlen(name));
	tmp->surface  = NULL;
	tmp->height   = 0;
	tmp->width    = 0;
	tmp->tileMap  = Tile_array_new();
	strcpy(tmp->name, name);
	return tmp;
}

Map * Map_load(char * name, Asset_array * assets)
{
	FILE * mapFile = NULL;
	char * mapFilename = NULL;
	char * buffer      = malloc(sizeof(char) * 255);
	char * orig_buffer = buffer;
	int    lineLen     = 0;
	Map  * map         = NULL;

	long int  * rawmap      = NULL;
	int    rawmapPos   = 0;
	SDL_Rect blitRect;
	int i, j;
	Uint8 tmpRGB[3];

	char * key = NULL;
	char * val = NULL;

	char * strIterator = NULL;

	bool inOptions  = false;
	bool inTiles    = false;
	bool inMap      = false;
	bool inMapLayer = false;

	// + 6 is for '.map', '/' and '\0'
	mapFilename = malloc(sizeof(char) * (MAPS_DIR_LEN + strlen(name) + 6));
	sprintf(mapFilename, "%s/%s.map", MAPS_DIR, name);
	mapFile = fopen(mapFilename, "r");

	if(mapFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", mapFilename);
		perror(NULL);
	}
	else
	{
		map = Map_init(name);
		while(fgets(buffer, 255, mapFile) != NULL)
		{
			trim(&buffer);

			if(buffer[0] != '#' && buffer[0] != '\0')
			{
				trimComment(&buffer);
				lineLen = strlen(buffer);

				#if defined(__DEBUG__) && defined(__DEBUG_MAP__)
					fprintf(stdout, )
				#endif

				if(buffer[0] == '[' && buffer[lineLen - 1] == ']')
				{
					// Extract the name
					buffer[lineLen - 1] = '\0';
					buffer = buffer + 1;

					if(strcmp(buffer, "options") == 0)
					{
						inOptions = true;
						inTiles   = false;
						inMap     = false;
					}
					else if(strcmp(buffer, "tiles") == 0)
					{
						inOptions = false;
						inTiles   = true;
						inMap     = false;
					}
					else if(strcmp(buffer, "map") == 0)
					{
						inOptions = false;
						inTiles   = false;
						inMap     = true;
					}
				}
				else if(inOptions)
				{
					// Extract key & value
					key = buffer;
					val = cut(key, '=');
					trim(&key);
					trim(&val);

					     if(strcmp(key, "width") == 0)  map->width = atoi(val);
					else if(strcmp(key, "height") == 0) map->height = atoi(val);
					else if(strcmp(key, "background_color") == 0)
					{
						strIterator = strtok(val, ",");
						i = 0;
						while(strIterator != NULL && i < 3)
						{
							tmpRGB[i] = atoi(strIterator);
							strIterator = strtok(NULL, ",");
							i++;
						}

						map->background_color.r = tmpRGB[0];
						map->background_color.g = tmpRGB[1];
						map->background_color.b = tmpRGB[2];
					}
					else fprintf(stderr, "Warning: parsing map '%s' : unknow option '%s = %s'\n", name, key, val);
				}
				else if(inTiles)
				{
					key = buffer;
					val = cut(key, '=');
					trim(&key);
					trim(&val);
					Tile_array_add(map->tileMap, atoi(val), Asset_get(assets, key));
				}
				else if(inMap)
				{
					if(map->width == 0 || map->height == 0)
					{
						fprintf(stderr, "Error: parsing map '%s' : height & width must be different from 0\n", name);
						return NULL;
					}

					if(rawmap == NULL)
						rawmap = calloc((sizeof(long int) * map->width * map->height) + 1, sizeof(long int));

					trim(&buffer);
					strIterator = strtok(buffer, ",");
					while(strIterator != NULL)
					{
						rawmap[rawmapPos++] = strtol(strIterator, NULL, 16);
						strIterator = strtok(NULL, ",");
					}
				}
			}
		}
	}

	fprintf(stdout, "Map render... "); fflush(stdout);
	if(!Map_render(map, rawmap))
	{
		fprintf(stderr, "Unable to render properly the map.\n");
		return NULL;
	}
	fprintf(stdout, "[OK]\n");

	free(mapFilename);
	free(orig_buffer);
	free(rawmap);

	return map;
}

bool Map_render(Map * map, long int * rawmap)
{
	Tile_array_elem_t * tile;
	SDL_Rect blitRect;
	int i, j;

	map->surface = SDL_CreateRGBSurface(0, map->width * TILE_WIDTH, map->height * TILE_HEIGHT, 32, RMASK, GMASK, BMASK, AMASK);
	if(map->surface == NULL)
	{
		fprintf(stderr, "Unable to create map rendering surface : %s\n", SDL_GetError());
		return false;
	}

	map->tiles = malloc(sizeof(Tile **) * map->height);

	blitRect.x = 0;
	blitRect.y = 0;
	blitRect.h = TILE_HEIGHT;
	blitRect.w = TILE_WIDTH;

	for(i = 0 ; i < map->height ; i++)
	{
		map->tiles[i] = malloc(sizeof(Tile *) * map->width);
		for(j = 0 ; j < map->width ; j++)
		{
			if(rawmap[(map->width * i) + j] != 0)
			{
				tile = Tile_array_get(map->tileMap, rawmap[(map->width * i) + j]);
				if(tile == NULL)
				{
					fprintf(stderr, "Unable to get tile #%d in map '%s' at [%d, %d]\n", rawmap[(map->width * i) + j]);
					return false;
				}
				map->tiles[i][j] = tile->value;
				if(SDL_BlitSurface(map->tiles[i][j]->surface, NULL, map->surface, &blitRect) != 0)
				{
					fprintf(stderr, "Unable to render tile[%d][%d] of the map '%s' : %s\n", i, j, map->name, SDL_GetError());
					return false;
				}
			}
			else
				map->tiles[i][j] = NULL;

			blitRect.x += TILE_WIDTH;
		}

		blitRect.x = 0;
		blitRect.y += TILE_HEIGHT;
	}

	return true;
}

void Map_free(Map * map)
{

}
