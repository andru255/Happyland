#include "game.h"

GameData * GameData_new()
{
	GameData * gd = calloc(sizeof(GameData), sizeof(GameData));

	assert(gd != NULL);

	// Load Assets
	fprintf(stdout, "[PENDING] Load assets"); fflush(stdout);
	gd->assets = Asset_load_config();
	if(gd->assets != NULL) fprintf(stdout, "\r[OK] Load assets     \n");
	else                   fprintf(stdout, "\r[ERROR] Load assets  \n");

	// Load Characters
	fprintf(stdout, "[PENDING] Load characters"); fflush(stdout);
	gd->characters = CharacterSurface_LoadList();
	if(gd->characters != NULL) fprintf(stdout, "\r[OK] Load characters     \n");
	else                       fprintf(stdout, "\r[ERROR] Load characters  \n");

	// Load Map list
	fprintf(stdout, "[PENDING] Load map list"); fflush(stdout);
	gd->maps = Map_load_list();
	if(gd->maps != NULL) fprintf(stdout, "\r[OK] Load map list     \n");
	else                 fprintf(stdout, "\r[ERROR] Load map list  \n");

	// Load theme
	fprintf(stdout, "[PENDING] Load theme music"); fflush(stdout);
	gd->theme = Mix_LoadMUS("./assets/music/theme.ogg");
	Game_PlayTheme(gd);
	Game_PauseTheme(gd);
	if(gd->theme != NULL) fprintf(stdout, "\r[OK] Load theme music     \n");
	else                  fprintf(stdout, "\r[ERROR] Load theme music  \n");

	gd->players = PlayerList_new();

	// Open Window
	gd->window = SDL_CreateWindow("Happyland",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		1366, 768,
		SDL_WINDOW_OPENGL
	);

	if (gd->window == NULL) {
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	gd->screen = SDL_GetWindowSurface(gd->window);

	return gd;
}

void GameData_free(GameData * gd)
{
	if(gd == NULL) return;

	fprintf(stdout, "[PENDING] Free Game data"); fflush(stdout);
	// Free the character list
	CharacterList_free(gd->characters);

	// Free the map list
	Map_list_free(gd->maps);

	// Free the asset list
	Asset_list_free(gd->assets);

	// Free music
	Mix_FreeMusic(gd->theme);

	// Free SDL stuff
	SDL_FreeSurface(gd->screen);
	SDL_DestroyWindow(gd->window);
	fprintf(stdout, "\r[OK] Free Game data       \n");
}

void Game_PlayTheme(GameData * gd)
{
	assert(gd != NULL);
	assert(gd->theme != NULL);
	Mix_PlayMusic(gd->theme, -1);
	gd->isThemePlaying = true;
}

void Game_ResumeTheme(GameData * gd)
{
	assert(gd != NULL);
	if(!gd->isThemePlaying)
	{
		gd->isThemePlaying = true;
		Mix_ResumeMusic();
	}
}

void Game_PauseTheme(GameData * gd)
{
	assert(gd != NULL);
	if(gd->isThemePlaying)
	{
		gd->isThemePlaying = false;
		Mix_PauseMusic();
	}
}

void Game_ToggleTheme(GameData * gd)
{
	assert(gd != NULL);
	if(gd->isThemePlaying)
		Game_PauseTheme(gd);
	else
		Game_ResumeTheme(gd);
}

void Game_LoadMap(GameData * gd, Map * map)
{
	assert(gd  != NULL);
	assert(map != NULL);
	assert(gd->assets != NULL);
	Map_load(map, gd->assets);
}

void Game_SelectMap(GameData * gd, char * mapName)
{
	assert(gd != NULL);
	assert(gd->maps != NULL);
	assert(mapName  != NULL);

	gd->currentMap     = Map_list_get(gd->maps, mapName)->value;
	gd->currentMapName = Map_list_get(gd->maps, mapName)->index;
	if(gd->currentMap)
	{
		if(!gd->currentMap->loaded)
			Game_LoadMap(gd, gd->currentMap);
	}
}

void Game_DisplayMap(GameData * gd, Map * map, bool updateWindow)
{
	assert(gd != NULL);
	assert(gd->screen != NULL);
	assert(gd->window != NULL);
	assert(map != NULL);
	assert(map->surface != NULL);

	SDL_FillRect(gd->screen,
		NULL,
		SDL_MapRGB(gd->screen->format,
			map->background_color.r,
			map->background_color.g,
			map->background_color.b
		)
	);
	SDL_BlitSurface(map->surface, NULL, gd->screen, NULL);

	if(updateWindow)
		SDL_UpdateWindowSurface(gd->window);
}

void Game_DisplayMapForeground(GameData * gd, Map * map, bool updateWindow)
{
	SDL_Rect blitRect;
	int i, j;
	assert(gd != NULL);
	assert(gd->screen != NULL);
	assert(gd->window != NULL);
	assert(map != NULL);
	assert(map->surface != NULL);

	blitRect.x = 0;
	blitRect.y = 0;
	blitRect.h = TILE_HEIGHT;
	blitRect.w = TILE_WIDTH;

	for(i = 0 ; i < map->height ; i++)
	{
		for(j = 0 ; j < map->width ; j++)
		{
			if(map->layers[i * TILE_HEIGHT][j * TILE_WIDTH] == L_FG_SOFT ||
			   map->layers[i * TILE_HEIGHT][j * TILE_WIDTH] == L_FG_SOLID)
				SDL_BlitSurface(map->tiles[i][j]->surface, NULL, gd->screen, &blitRect);

			blitRect.x += TILE_WIDTH;
		}

		blitRect.x = 0;
		blitRect.y += TILE_HEIGHT;
	}

	if(updateWindow)
		SDL_UpdateWindowSurface(gd->window);
}

Player * Game_AddPlayer(GameData * gd, char * name, char * character)
{
	Player * tmp = NULL;
	PlayerList_elem_t * it = NULL;
	CharacterList_elem_t * charSurface = NULL;

	assert(gd != NULL);
	assert(gd->players != NULL);
	assert(gd->characters != NULL);

	charSurface = CharacterList_get(gd->characters, character);
	if(charSurface == NULL)
		return NULL;

	it = PlayerList_get(gd->players, name);
	if(it != NULL)
	{
		tmp = it->value;
		Player_SetCharacter(tmp, charSurface->value);
		return tmp;
	}

	tmp = Player_new(name);
	Player_SetCharacter(tmp, charSurface->value);
	return PlayerList_add(gd->players, name, tmp)->value;
}

Player * Game_GetPlayer(GameData * gd, char * name)
{
	PlayerList_elem_t * tmp = NULL;

	assert(gd != NULL);
	assert(gd->players != NULL);

	tmp = PlayerList_get(gd->players, name);
	if(tmp != NULL)
		return tmp->value;
	return NULL;
}

void Game_RemovePlayer(GameData * gd, char * name)
{
	assert(gd != NULL);
	assert(gd->players != NULL);
	PlayerList_remove(gd->players, name);
}

void Game_SetPlayerPositionOnMap(GameData * gd, char * playerName, int x, int y)
{
	Player * player = NULL;

	assert(gd != NULL);

	player = Game_GetPlayer(gd, playerName);
	if(player == NULL)
	{
		fprintf(stderr, "Error: No player named : %s\n", playerName);
	}
	else
	{
		player->position.x = (x * TILE_WIDTH);
		player->position.y = ((y * TILE_HEIGHT) + TILE_HEIGHT) - player->character->size.h;
	}
}

void Game_UpdateDisplay(GameData * gd)
{
	PlayerList_elem_t * playerIt = NULL;
	SDL_Rect blitRect;

	assert(gd != NULL);
	assert(gd->currentMap != NULL);

	// First redraw the map
	Game_DisplayMap(gd, gd->currentMap, false);

	// Redraw all players
	for(playerIt = gd->players->array ; playerIt != NULL ; playerIt = playerIt->next)
	{
		Player_UpdateSurface(playerIt->value, gd->currentMap);
		//fprintf(stdout, "Draw player '%s' at [%4d,%4d], speed [%3d, %3d]\n", playerIt->value->name, playerIt->value->position.x, playerIt->value->position.y, playerIt->value->speed.x, playerIt->value->speed.y);
		SDL_BlitSurface(playerIt->value->surface, NULL, gd->screen, &playerIt->value->position);
		Player_ResetSpeed(playerIt->value);
		//Player_AddOverlay(playerIt->value, gd->screen);
	}

	Game_DisplayMapForeground(gd, gd->currentMap, false);
	//Map_AddLayerOverlay(gd->currentMap, gd->screen);

	// Update the window
	SDL_UpdateWindowSurface(gd->window);
}
