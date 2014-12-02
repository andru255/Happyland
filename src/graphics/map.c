#include "map.h"

IMPLEMENT_LIST(Map_list,  Map *,  char *, Map_copy, Str_copy, Map_cmp, Str_cmp, Map_free, Str_free);

Map * Map_init(char * name, char * filename)
{
	#ifdef __DEBUG_MAP_FN__
	fprintf(stdout, "Map_init(%s)\n", name);
	#endif
	Map * tmp     = malloc(sizeof(Map));
	tmp->tiles    = NULL;
	tmp->name     = calloc(sizeof(char) * strlen(name), sizeof(char));
	tmp->filename = calloc(sizeof(char) * strlen(filename), sizeof(char));
	tmp->surface  = NULL;
	tmp->height   = 0;
	tmp->width    = 0;
	tmp->loaded   = false;
	tmp->tileMap  = Tile_list_new();
	tmp->tileMap->freeValue = 0;
	strcpy(tmp->name, name);
	strcpy(tmp->filename, filename);
	return tmp;
}

Map * Map_init_load(char * name, char * filename, Asset_list * assets)
{
	#ifdef __DEBUG_MAP_FN__
	fprintf(stdout, "Map_init_load(%s, Asset_list[%d])\n", name, assets->size);
	#endif
	Map * tmp = Map_init(name, filename);
	tmp = Map_load(tmp, assets);
	return tmp;
}

Map * Map_load(Map * map, Asset_list * assets)
{
	#ifdef __DEBUG_MAP_FN__
	fprintf(stdout, "Map_load(%s, Asset_list[%d])\n", map->name, assets->size);
	#endif
	FILE * mapFile     = NULL;
	char * mapFilename = NULL;
	char * buffer      = malloc(sizeof(char) * 255);
	char * orig_buffer = buffer;
	int    lineLen     = 0;

	long int  * rawmap    = NULL;
	int         rawmapPos = 0;

	int * rawmaplayer    = NULL;
	int   rawmaplayerPos = 0;

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

	#ifdef __DEBUG_MAP_LOAD__
	int line_number;
	#endif

	// If map is already loaded
	// doesn't reload it
	if(map->loaded) return map;

	// + 6 is for '.map', '/' and '\0'
	mapFilename = malloc(sizeof(char) * (MAPS_DIR_LEN + strlen(map->name) + 6));
	sprintf(mapFilename, "%s/%s", MAPS_DIR, map->filename);
	mapFile = fopen(mapFilename, "r");

	if(mapFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", mapFilename);
		perror(NULL);
	}
	else
	{
		#ifdef __DEBUG_MAP_LOAD__
		line_number = 0;
		#endif
		while(fgets(buffer, 255, mapFile) != NULL)
		{
			trim(&buffer);

			if(buffer[0] != '#' && buffer[0] != '\0')
			{
				trimComment(&buffer);
				lineLen = strlen(buffer);

				#if defined(__DEBUG_MAP_LOAD__) && defined(__DEBUG_PARSING_SHOW_LINES__)
				fprintf(stdout, "Line %04d: %s\n", line_number++, buffer);
				#endif

				if(buffer[0] == '[' && buffer[lineLen - 1] == ']')
				{
					// Extract the name
					buffer[lineLen - 1] = '\0';
					buffer = buffer + 1;

					if(strcmp(buffer, "options") == 0)
					{
						#ifdef __DEBUG_MAP_LOAD__
						fprintf(stdout, "\nMap_load: [options]\n");
						#endif
						inOptions  = true;
						inTiles    = false;
						inMap      = false;
						inMapLayer = false;
					}
					else if(strcmp(buffer, "tiles") == 0)
					{
						#ifdef __DEBUG_MAP_LOAD__
						fprintf(stdout, "\nMap_load: [tiles]\n");
						#endif
						inOptions  = false;
						inTiles    = true;
						inMap      = false;
						inMapLayer = false;
					}
					else if(strcmp(buffer, "map") == 0)
					{
						#ifdef __DEBUG_MAP_LOAD__
						fprintf(stdout, "\nMap_load: [map]\n");
						#endif
						inOptions  = false;
						inTiles    = false;
						inMap      = true;
						inMapLayer = false;
					}
					else if(strcmp(buffer, "map-layers") == 0)
					{
						#ifdef __DEBUG_MAP_LOAD__
						fprintf(stdout, "\nMap_load: [map-layers]\n");
						#endif
						inOptions  = false;
						inTiles    = false;
						inMap      = false;
						inMapLayer = true;
					}
					else fprintf(stdout, "Warning: parsing map '%s' : unknow block : [%s]\n", map->name, buffer);
				}
				else if(inOptions)
				{
					// Extract key & value
					key = buffer;
					val = cut(key, '=');
					trim(&key);
					trim(&val);

					#ifdef __DEBUG_MAP_LOAD__
					fprintf(stdout, "Map_load: %s = %s\n", key, val);
					#endif

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
					else fprintf(stderr, "Warning: parsing map '%s' : unknow option '%s = %s'\n", map->name, key, val);
				}
				else if(inTiles)
				{
					key = buffer;
					val = cut(key, '=');
					trim(&key);
					trim(&val);

					#ifdef __DEBUG_MAP_LOAD__
					fprintf(stdout, "Map_load: %s = %d\n", key, (int)strtol(val, NULL, 16));
					#endif

					//if(Asset_get(assets, key) != NULL)
						Tile_list_add(map->tileMap, (int)strtol(val, NULL, 16), Asset_get(assets, key));
					//else
					//	fprintf(stderr, "Unable to get tile\n");
				}
				else if(inMap)
				{
					if(map->width == 0 || map->height == 0)
					{
						fprintf(stderr, "Error: parsing map '%s' : height & width must be different from 0\n", map->name);
						return NULL;
					}

					if(rawmap == NULL)
						rawmap = calloc((sizeof(long int) * map->width * map->height) + 1, sizeof(long int));

					trim(&buffer);

					#ifdef __DEBUG_MAP_LOAD__
					fprintf(stdout, "Map_load: ");
					fflush(stdout);
					#endif

					strIterator = strtok(buffer, ",");
					while(strIterator != NULL)
					{
						rawmap[rawmapPos++] = strtol(strIterator, NULL, 16);
						#ifdef __DEBUG_MAP_LOAD__
						if(rawmap[rawmapPos - 1] == 0)
							fprintf(stdout, "   ,");
						else
							fprintf(stdout, "%3d,", rawmap[rawmapPos - 1]);
						fflush(stdout);
						#endif
						strIterator = strtok(NULL, ",");
					}

					#ifdef __DEBUG_MAP_LOAD__
					fprintf(stdout, "\n");
					#endif
				}
				else if(inMapLayer)
				{
					if(map->width == 0 || map->height == 0)
					{
						fprintf(stderr, "Error: parsing map '%s' : height & width must be different from 0\n", map->name);
						return NULL;
					}

					if(rawmaplayer == NULL)
						rawmaplayer = calloc((sizeof(int) * map->width * map->height) + 1, sizeof(int));

					trim(&buffer);

					#ifdef __DEBUG_MAP_LOAD__
					fprintf(stdout, "Map_load: ");
					fflush(stdout);
					#endif

					strIterator = strtok(buffer, ",");
					while(strIterator != NULL)
					{
						rawmaplayer[rawmaplayerPos++] = (int)strtol(strIterator, NULL, 16);
						#ifdef __DEBUG_MAP_LOAD__
						if(rawmaplayer[rawmaplayerPos - 1] == 0)
							fprintf(stdout, "   ,");
						else
							fprintf(stdout, "%3d,", rawmaplayer[rawmaplayerPos - 1]);
						fflush(stdout);
						#endif
						strIterator = strtok(NULL, ",");
					}

					#ifdef __DEBUG_MAP_LOAD__
					fprintf(stdout, "\n");
					#endif
				}
			}
		}
	}

	if(!Map_render(map, rawmap, rawmaplayer))
	{
		fprintf(stderr, "Unable to render properly the map.\n");
		return NULL;
	}

	free(mapFilename);
	free(orig_buffer);
	free(rawmap);
	free(rawmaplayer);

	map->loaded = true;
	return map;
}

bool Map_render(Map * map, long int * rawmap, int * rawmaplayer)
{
	Tile_list_elem_t * tile;
	SDL_Rect blitRect;
	int i, j;

	map->surface = SDL_CreateRGBSurface(0, map->width * TILE_WIDTH, map->height * TILE_HEIGHT, 32, RMASK, GMASK, BMASK, AMASK);
	if(map->surface == NULL)
	{
		fprintf(stderr, "Unable to create map rendering surface : %s\n", SDL_GetError());
		return false;
	}

	map->tiles  = calloc(sizeof(Tile **) * map->height, sizeof(Tile **));
	map->layers = calloc(sizeof(int)     * map->height, sizeof(int));

	blitRect.x = 0;
	blitRect.y = 0;
	blitRect.h = TILE_HEIGHT;
	blitRect.w = TILE_WIDTH;

	for(i = 0 ; i < map->height ; i++)
	{
		map->tiles[i]  = calloc(sizeof(Tile *) * map->width, sizeof(Tile *));
		map->layers[i] = calloc(sizeof(int)    * map->width, sizeof(int));

		for(j = 0 ; j < map->width ; j++)
		{
			// Fill map->tiles
			if(rawmap[(map->width * i) + j] != 0)
			{
				tile = Tile_list_get(map->tileMap, rawmap[(map->width * i) + j]);
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

			// Fill map->layers
			map->layers[i][j] = rawmaplayer[(map->width * i) + j];

			blitRect.x += TILE_WIDTH;
		}

		blitRect.x = 0;
		blitRect.y += TILE_HEIGHT;
	}

	return true;
}

void Map_free(Map * map)
{
	int i;
	for(i = 0 ; i < map->height ; i++)
	{
		if(map->tiles[i])
			free(map->tiles[i]);

		if(map->layers[i])
			free(map->layers[i]);
	}

	free(map->tiles);
	free(map->layers);

	SDL_FreeSurface(map->surface);

	Tile_list_free(map->tileMap);

	free(map->name);
	free(map->filename);

	free(map);
}

void Map_copy(Map ** dest, Map ** src)
{
	*dest = malloc(sizeof(Map));
	memcpy(*dest, *src, sizeof(Map));
}

int Map_cmp(Map * val1, Map * val2)
{
	return memcmp(val1, val2, sizeof(Map));
}

Map_list * Map_load_list()
{
	Map_list * maps        = NULL;
	FILE     * listFile    = NULL;
	char     * buffer      = calloc(sizeof(char) * 255, sizeof(char));
	char     * orig_buffer = buffer;
	char     * mapFilename = NULL;
	char     * mapName     = NULL;

	listFile = fopen(MAPS_LIST_FILE, "r");

	if(listFile == NULL)
	{
		fprintf(stderr, "Unable to open %s : ", MAPS_LIST_FILE);
		perror(NULL);
	}
	else
	{
		maps = Map_list_new();
		while(fgets(buffer, 255, listFile) != NULL)
		{

			trim(&buffer);
			if(buffer[0] != '#' && buffer[0] != '\0')
			{
				mapFilename = buffer;
				mapName = cut(mapFilename, '=');
				trim(&mapFilename);
				trim(&mapName);

				// Load and add the map to the list
				Map_list_add(maps, mapName, Map_init(mapName, mapFilename));
			}
		}
	}

	free(orig_buffer);
	fclose(listFile);
	return maps;
}