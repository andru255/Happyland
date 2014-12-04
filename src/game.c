#include "game.h"

GameData * GameData_new()
{
	GameData * gd = malloc(sizeof(GameData));

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
	fprintf(stdout, "\r[OK] Free Game data     \n");
}

void Game_PlayTheme(GameData * gd)
{
	Mix_PlayMusic(gd->theme, -1);
	gd->isThemePlaying = true;
}

void Game_ResumeTheme(GameData * gd)
{
	if(!gd->isThemePlaying)
	{
		gd->isThemePlaying = true;
		Mix_ResumeMusic();
	}
}

void Game_PauseTheme(GameData * gd)
{
	if(gd->isThemePlaying)
	{
		gd->isThemePlaying = false;
		Mix_PauseMusic();
	}
}

void Game_ToggleTheme(GameData * gd)
{
	if(gd->isThemePlaying)
		Game_PauseTheme(gd);
	else
		Game_ResumeTheme(gd);
}

void Game_LoadMap(GameData * gd, Map * map)
{
	Map_load(map, gd->assets);
}

void Game_SelectMap(GameData * gd, char * mapName)
{
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

Player * Game_AddPlayer(GameData * gd, char * name, char * character)
{
	Player * tmp = NULL;
	PlayerList_elem_t * it = NULL;
	CharacterList_elem_t * charSurface = NULL;

	charSurface = CharacterList_get(gd->characters, character);
	if(charSurface == NULL)
		return NULL;

	it = PlayerList_get(gd->players, name);
	if(it != NULL)
	{
		tmp = it->value;
		tmp->character = charSurface->value;
		tmp->surface   = CharacterSurface_get(charSurface->value, CS_DEFAULT, CO_RIGHT);
		return tmp;
	}

	tmp = Player_new(name);
	tmp->character = charSurface->value;
	tmp->surface   = CharacterSurface_get(charSurface->value, CS_DEFAULT, CO_RIGHT);
	return PlayerList_add(gd->players, name, tmp)->value;
}

Player * Game_GetPlayer(GameData * gd, char * name)
{
	PlayerList_elem_t * tmp = NULL;
	tmp = PlayerList_get(gd->players, name);
	if(tmp != NULL)
		return tmp->value;
	return NULL;
}

void Game_RemovePlayer(GameData * gd, char * name)
{
	PlayerList_remove(gd->players, name);
}

void Game_SetPlayerPositionOnMap(GameData * gd, char * playerName, int x, int y)
{
	Player * player = NULL;

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
	// First redraw the map
	Game_DisplayMap(gd, gd->currentMap, false);

	// Redraw all players
	for(playerIt = gd->players->array ; playerIt != NULL ; playerIt = playerIt->next)
	{
		Player_UpdateSurface(playerIt->value);
		Player_GetBlitRect(playerIt->value, &blitRect);
		SDL_BlitSurface(playerIt->value->surface, NULL, gd->screen, &blitRect);
	}

	// Update the window
	SDL_UpdateWindowSurface(gd->window);
}