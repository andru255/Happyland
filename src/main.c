#include <stdio.h>
#include "game.h"

int main(int argc, char ** argv)
{
	GameData  * gd = NULL;
	Player    * player = NULL;
	CharacterSurface * dog = NULL;
	SDL_Rect blitRect;
	int i, j;

	// Initialize graphics stack
	if(Graphics_Init() != 0)
	{
		fprintf(stdout, "Unable to initialize graphics stack. Exiting.\n");
		exit(ERR_GRAPHICS_STACK_INIT);
	}
	// Register graphics stack close on exit
	atexit(Graphics_Close);

	gd = GameData_new();
	Game_LoadMap(gd, "Example Map");

	dog = CharacterSurface_new();
	if(!CharacterSurface_load(dog, "./assets/characters/player.character"))
	{
		fprintf(stderr, "Unable to load DOG\n");
		exit(-6);
	}

	player = Player_new();
	player->surface = CharacterSurface_get(dog, CS_DEFAULT);

	SDL_BlitSurface(player->surface, NULL, gd->screen, NULL);
	SDL_UpdateWindowSurface(gd->window);

	pause();

	GameData_free(gd);

	CharacterSurface_free(dog);
	Player_free(player);

	return ERR_SUCCESS;
}