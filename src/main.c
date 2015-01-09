#include <stdio.h>
#include "game.h"

int main(int argc, char ** argv)
{
	GameData  * gd = NULL;
	Player    * player = NULL;
	SDL_Rect destRect;
	bool continuer = true;
	SDL_Event event;

	bool goLeft   = false;
	bool goRight  = false;
	bool goUp     = false;

	float upCpt = 0;

	// Initialize graphics stack
	if(Graphics_Init() != 0)
	{
		fprintf(stdout, "Unable to initialize graphics stack. Exiting.\n");
		exit(ERR_GRAPHICS_STACK_INIT);
	}
	// Register graphics stack close on exit
	atexit(Graphics_Close);

	gd = GameData_new();

	Game_SelectMap(gd, "Example Map");
	Game_DisplayMap(gd, gd->currentMap, false);

	Game_AddPlayer(gd, "Player1", "The Dog");
	player = Game_GetPlayer(gd, "Player1");

	Game_SetPlayerPositionOnMap(gd, "Player1", gd->currentMap->startPos.x, gd->currentMap->startPos.y);

	Game_UpdateDisplay(gd);

	while (continuer)
	{
		if(SDL_PollEvent(&event) == 1)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					continuer = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_RIGHT:
							goRight = true;
							break;

						case SDLK_LEFT:
							goLeft = true;
							break;

						case SDLK_UP:
							goUp = true;
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_RIGHT:
							goRight = false;
							break;

						case SDLK_LEFT:
							goLeft  = false;
							break;

						case SDLK_UP:
							goUp = false;
							break;

						case SDLK_p:
							Game_ToggleTheme(gd);
							break;
					}
					break;
			}
		}

		if(goLeft && !goRight)
		{
			player->speed.x = -5;
		}

		if(goRight && !goLeft)
		{
			player->speed.x = 5;
		}

		if(goUp)
		{
			if(upCpt == 0)
				player->inJump = true;
			goUp = false;
		}

		if(player->inJump)
		{
			if(upCpt <= 2.0*sqrt(TILE_HEIGHT*3.0))
			{
				//jump = true;
				player->speed.y = -(int)(2.0*(sqrt(TILE_HEIGHT*3.0)-upCpt));
				upCpt += 1;
			}
			else
			{
				player->inJump  = false;
				upCpt           = 0;
			}
		}

		Game_UpdateDisplay(gd);
		SDL_Delay(25);
	}

	GameData_free(gd);

	return ERR_SUCCESS;
}