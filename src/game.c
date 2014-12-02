#include "game.h"

GameData * GameData_new()
{
	GameData * gd = malloc(sizeof(GameData));

	// Load game data
	fprintf(stdout, "[PENDING] Load assets"); fflush(stdout);
	gd->assets = Asset_load_config();
	if(gd->assets != NULL) fprintf(stdout, "\r[OK] Load assets     \n");
	else                   fprintf(stdout, "\r[ERROR] Load assets  \n");

	// Load Map list
	fprintf(stdout, "[PENDING] Load map list"); fflush(stdout);
	gd->maps = Map_load_list();
	if(gd->maps != NULL) fprintf(stdout, "\r[OK] Load map list     \n");
	else                 fprintf(stdout, "\r[ERROR] Load map list  \n");

	// Start Game
	gd->window = SDL_CreateWindow("Happyland",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
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
	Map_list_free(gd->maps);
	Asset_list_free(gd->assets);
	SDL_FreeSurface(gd->screen);
	SDL_DestroyWindow(gd->window);
	fprintf(stdout, "\r[OK] Free Game data     \n");
}

void Game_LoadMap(GameData * gd, char * mapName)
{
	Map_list_elem_t * map;
	map = Map_list_get(gd->maps, mapName);
	Map_load(map->value, gd->assets);

	SDL_FillRect(gd->screen,
		NULL,
		SDL_MapRGB(gd->screen->format,
			map->value->background_color.r,
			map->value->background_color.g,
			map->value->background_color.b
		)
	);

	SDL_BlitSurface(map->value->surface, NULL, gd->screen, NULL);
	SDL_UpdateWindowSurface(gd->window);
}