#include <stdio.h>
#include "game.h"

int main(int argc, char ** argv)
{
	//Array    * assets;
	GameData * gd;
	int i, j;
	Asset_list_elem_t * assetIt;
	Tile_list_elem_t * tileIt;

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

	gd = GameData_new();

	for(assetIt = gd->assets->array ; assetIt != NULL ; assetIt = assetIt->next)
	{
		fprintf(stdout, "Asset: %s\n", assetIt->value->name);
		for(tileIt = assetIt->value->tiles->array ; tileIt != NULL ; tileIt = tileIt->next)
			fprintf(stdout, "\tTile: %s - loaded: %s\n", tileIt->value->name, tileIt->value->loaded ? "true":"false");
	}

	// Load Map list
	fprintf(stdout, "[PENDING] Load map list"); fflush(stdout);
	exampleMap = Map_init_load("example", gd->assets);
	if(exampleMap != NULL) fprintf(stdout, "\r[OK] Load map list     \n");
	else                   fprintf(stdout, "\r[ERROR] Load map list     \n");

	SDL_FillRect(gd->screen,NULL, SDL_MapRGB(gd->screen->format, exampleMap->background_color.r, exampleMap->background_color.g, exampleMap->background_color.b));
	SDL_BlitSurface(exampleMap->surface, NULL, gd->screen, NULL);
	SDL_UpdateWindowSurface(gd->window);

	pause();

	SDL_DestroyWindow(gd->window);

	// Free Game Data
	fprintf(stdout, "Map_free... "); fflush(stdout);
	Map_free(exampleMap);
	fprintf(stdout, "[OK]\n");
	fprintf(stdout, "Asset_array_free... "); fflush(stdout);
	Asset_list_free(gd->assets);
	fprintf(stdout, "[OK]\n");
	return ERR_SUCCESS;
}