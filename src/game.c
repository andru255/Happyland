#include "game.h"

#include "tinydir.h"

GameData * GameData_new()
{
	GameData * gd = malloc(sizeof(GameData));

	// Load game data
	fprintf(stdout, "[PENDING] Load assets"); fflush(stdout);
	gd->assets = Asset_load_config();
	fprintf(stdout, "\r[OK] Load assets     \n");

	// Load Map list
	fprintf(stdout, "[PENDING] Load map list"); fflush(stdout);
	//exampleMap = Map_init_load("example", gd->assets);

	if(exampleMap != NULL) fprintf(stdout, "[OK]\n");
	else                   fprintf(stdout, "[ERROR]\n");

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