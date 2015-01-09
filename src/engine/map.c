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
	tmp->startPos.x = -1;
	tmp->startPos.y = -1;
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

	int i;
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
	mapFilename = calloc(sizeof(char) * (MAPS_DIR_LEN + strlen(map->name) + 6), sizeof(char));
	sprintf(mapFilename, "%s/%s", MAPS_DIR, map->filename);
	mapFile = fopen(mapFilename, "r");
	free(mapFilename);

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
					else if(strcmp(key, "edge_left")   == 0) map->edge.left   = Map_ConvertEdgeToLayer(val);
					else if(strcmp(key, "edge_right")  == 0) map->edge.right  = Map_ConvertEdgeToLayer(val);
					else if(strcmp(key, "edge_top")    == 0) map->edge.top    = Map_ConvertEdgeToLayer(val);
					else if(strcmp(key, "edge_bottom") == 0) map->edge.bottom = Map_ConvertEdgeToLayer(val);
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
							fprintf(stdout, "%3ld,", rawmap[rawmapPos - 1]);
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

	fclose(mapFile);
	free(orig_buffer);
	free(rawmap);
	free(rawmaplayer);

	map->loaded = true;
	return map;
}

bool Map_render(Map * map, long int * rawmap, int * rawmaplayer)
{
	Tile_list_elem_t * tile;
	Layer tmpLayer;
	TileShape tmpShape;
	SDL_Rect blitRect;
	int i, j;
	bool startPosDefined = false;

	map->surface = SDL_CreateRGBSurface(0, map->width * TILE_WIDTH, map->height * TILE_HEIGHT, 32, RMASK, GMASK, BMASK, AMASK);
	if(map->surface == NULL)
	{
		fprintf(stderr, "Unable to create map rendering surface : %s\n", SDL_GetError());
		return false;
	}

	map->tiles  = calloc(sizeof(Tile **) * map->height, sizeof(Tile **));
	map->layers = calloc(sizeof(Layer *) * map->height * TILE_HEIGHT, sizeof(Layer *));

	j = 0;
	for(i = 0 ; i < map->height * TILE_HEIGHT ; i++)
		map->layers[i] = calloc(sizeof(Layer) * map->width * TILE_WIDTH, sizeof(Layer));

	blitRect.x = 0;
	blitRect.y = 0;
	blitRect.h = TILE_HEIGHT;
	blitRect.w = TILE_WIDTH;

	for(i = 0 ; i < map->height ; i++)
	{
		map->tiles[i]  = calloc(sizeof(Tile *) * map->width, sizeof(Tile *));

		for(j = 0 ; j < map->width ; j++)
		{
			// Fill map->tiles
			if(rawmap[(map->width * i) + j] != 0)
			{
				tile = Tile_list_get(map->tileMap, rawmap[(map->width * i) + j]);
				if(tile == NULL)
				{
					fprintf(stderr, "Unable to get tile #%ld in map '%s' at [%d, %d]\n", rawmap[(map->width * i) + j], map->name, i, j);
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
			tmpLayer = rawmaplayer[(map->width * i) + j];
			tmpShape = map->tiles[i][j] == NULL ? TS_FULL : map->tiles[i][j]->shape;
			Layer_FillPixels(map->layers, j * TILE_WIDTH, i * TILE_HEIGHT, tmpLayer, tmpShape);
			if(tmpLayer == 0xB)
			{
				map->startPos.x = j;
				map->startPos.y = i;
				startPosDefined = true;
			}

			blitRect.x += TILE_WIDTH;
		}

		blitRect.x = 0;
		blitRect.y += TILE_HEIGHT;
	}

	if(!startPosDefined)
		fprintf(stderr, "Warning: Map %s, no start point defined.", map->name);

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
	assert(*src != NULL);
	*dest = malloc(sizeof(Map));
	memcpy(*dest, *src, sizeof(Map));
}

int Map_cmp(Map * val1, Map * val2)
{
	assert(val1 != NULL && val2 != NULL);
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

Layer Map_GetPixelLayer(Map * map, int x, int y)
{
	assert(map != NULL);
	if(x < map->width * TILE_WIDTH   && x > -1 &&
	   y < map->height * TILE_HEIGHT && y > -1)
		return map->layers[y][x];
	return L_BG_SOLID;
}

bool Layer_IsSolid(Layer layer)
{
	static bool layerSolidity[] = {
		false,  // 0. Background not solid
		false,  // 1. Foreground not solid
		true,   // 2. Background solid
		true,   // 3. Foreground solid
		true,   // 4. Death
		true,   // 5. Ice
		false,  // 6. Exit
		true,   // 7. Door
		false,  // 8. Safe
		true,   // 9. Switch
		false,  // A. Swap
		false   // B. Start
	};

	return layerSolidity[layer];
}

void Layer_FillPixels(Layer ** layerMap, int x, int y, Layer value, TileShape shape)
{
	int i, j;

	switch(shape)
	{
		case TS_MID_LEFT:
			for(j = 0 ; j < TILE_HEIGHT ; j++)
				for(i = 0 ; i < TILE_WIDTH / 2 ; i++)
					layerMap[y + j][x + i] = value;
		break;

		case TS_MID_RIGHT:
			for(j = 0 ; j < TILE_HEIGHT ; j++)
				for(i = TILE_WIDTH / 2 ; i < TILE_WIDTH ; i++)
					layerMap[y + j][x + i] = value;
		break;

		default:
			for(j = 0 ; j < TILE_HEIGHT ; j++)
				for(i = 0 ; i < TILE_WIDTH ; i++)
					layerMap[y + j][x + i] = value;
		break;
	}
}

char * Layer_GetName(Layer layer)
{
	static char layerString[][11] = {
		"L_BG_SOFT ",
		"L_FG_SOFT ",
		"L_BG_SOLID",
		"L_FG_SOLID",
		"L_DEATH   ",
		"L_ICE     ",
		"L_EXIT    ",
		"L_DOOR    ",
		"L_SAFE    ",
		"L_SWITCH  ",
		"L_SWAP    ",
		"L_START   "
	};

	return layerString[layer];
}

char * Layer_GetDescription(Layer layer)
{
	static char layerString[][21] = {
		"Background not solid",
		"Foreground not solid",
		"Background solid",
		"Foreground solid",
		"Death",
		"Ice",
		"Exit",
		"Door",
		"Safe",
		"Switch",
		"Swap",
		"Start"
	};

	return layerString[layer];
}

void Map_AddLayerOverlay(Map * map, SDL_Surface * screen)
{
	static SDL_Color colors[] = {
		{0xFF, 0xFF, 0xFF, 0x00},  // 0. Background not solid
		{0x00, 0x00, 0xFF, 0x50},  // 1. Foreground not solid
		{0x00, 0xFF, 0x00, 0x50},  // 2. Background solid
		{0x00, 0xFF, 0x00, 0x50},  // 3. Foreground solid
		{0xFF, 0x00, 0x00, 0x50},  // 4. Death
		{0xFF, 0xFF, 0xFF, 0x00},  // 5. Ice
		{0xFF, 0xFF, 0xFF, 0x00},  // 6. Exit
		{0xFF, 0xFF, 0xFF, 0x00},  // 7. Door
		{0xFF, 0xFF, 0xFF, 0x00},  // 8. Safe
		{0xFF, 0xFF, 0xFF, 0x00},  // 9. Switch
		{0xFF, 0xFF, 0xFF, 0x00},  // A. Swap
		{0xFF, 0xFF, 0xFF, 0x00}   // B. Start
	};
	SDL_Rect pt;
	SDL_Surface * overlay = SDL_CreateRGBSurface(0, map->width * TILE_WIDTH, map->height * TILE_HEIGHT, 32, RMASK, GMASK, BMASK, AMASK);
	pt.h = TILE_HEIGHT;
	pt.w = TILE_WIDTH;

	for(pt.x = 0 ; pt.x < map->width * TILE_WIDTH ; pt.x += TILE_WIDTH)
		for(pt.y = 0 ; pt.y < map->height * TILE_HEIGHT; pt.y += TILE_HEIGHT)
			SDL_FillRect(overlay, &pt,
				SDL_MapRGBA(
					overlay->format,
					colors[map->layers[pt.y][pt.x]].r,
					colors[map->layers[pt.y][pt.x]].g,
					colors[map->layers[pt.y][pt.x]].b,
					colors[map->layers[pt.y][pt.x]].a
				)
			);

	SDL_BlitSurface(overlay, NULL, screen, NULL);
	SDL_FreeSurface(overlay);
}

Layer Map_ConvertEdgeToLayer(char * value)
{
	if(strcmp(value, "SOLID") == 0)
		return L_BG_SOLID;
	else if(strcmp(value, "DEATH") == 0)
		return L_DEATH;
	return L_BG_SOFT;
}