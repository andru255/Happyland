#include <stdio.h>
#include "game.h"

int main(int argc, char ** argv)
{
	//Array    * assets;
	GameData   gameData;
	int i, j;

	// Test variables
	SDL_Window  * window;
	SDL_Surface * screen;
	Map * exampleMap = NULL;

	// Initialize graphics stack
	if(Graphics_Init() != 0)
	{
		fprintf(stdout, "Unable to initialize graphics stack. Exiting.\n");
		exit(ERR_GRAPHICS_STACK_INIT);
	}
	// Register graphics stack close on exit
	atexit(Graphics_Close);

	// Load game data
	fprintf(stdout, "Asset_load_config... "); fflush(stdout);
	gameData.assets = Asset_load_config();
	fprintf(stdout, "[OK]\n");

	// Load example Map
	fprintf(stdout, "Map_load... "); fflush(stdout);
	exampleMap = Map_load("example", gameData.assets);

	if(exampleMap != NULL) fprintf(stdout, "[OK]\n");
	else                   fprintf(stdout, "[ERROR]\n");

	// Start Game
	window = SDL_CreateWindow("Happyland",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL) {
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, exampleMap->background_color.r, exampleMap->background_color.g, exampleMap->background_color.b));
	SDL_BlitSurface(exampleMap->surface, NULL, screen, NULL);
	SDL_UpdateWindowSurface(window);

	pause();

	SDL_DestroyWindow(window);

	// Free Game Data
	Asset_array_free(gameData.assets);
	return ERR_SUCCESS;
}