#include <stdio.h>
#include "game.h"

int main(int argc, char ** argv)
{
	GameData  * gd = NULL;
	Player    * player = NULL;
	SDL_Rect destRect;
	int walkCpt, fallCpt;
	bool continuer = true;
	unsigned int lastTick      = 0;
	unsigned int currentTick = 0;
	SDL_Event event;

	bool goLeft   = false;
	bool goRight  = false;
	bool goUp     = false;
	bool walk     = false;
	bool jump     = false;
	bool fall     = false;
	bool inJump   = false;
	bool inFall   = false;
	bool gravity  = false;
	bool startJumping = false;
	bool resetDog = false;

	unsigned int startJump_pos = 0;
	unsigned int maxJump_pos = 0;

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

	walkCpt = 0;

	while (continuer)
	{
		lastTick = SDL_GetTicks();
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
							walkCpt = 0;
							resetDog = true;
							break;

						case SDLK_LEFT:
							goLeft = false;
							walkCpt = 0;
							resetDog = true;
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
			player->position.x -= 5;
			walk = true;
		}

		if(goRight && !goLeft)
		{
			player->position.x += 5;
			walk = true;
		}

		if(goUp)
		{
			if(!inJump && !inFall)
			{
				inJump = true;
				startJump_pos = player->position.y;
				maxJump_pos   = player->position.y - (TILE_HEIGHT * 2);
			}
		}

		if(inJump)
		{
			if(player->position.y > maxJump_pos)
			{
				player->position.y -= 10;
				jump = true;
			}
			else
			{
				inFall   = true;
				inJump   = false;
				fall     = false;
				resetDog = true;
			}
		}

		if(inFall)
		{
			if(player->position.y < startJump_pos)
			{
				player->position.y += 10;
				fall    = true;
				fallCpt = 0;
			}
			else
			{
				fall     = false;
				resetDog = true;
				if(!goUp)
				{
					inFall   = false;
					inJump   = false;
				}
			}
		}

		if(goLeft)  player->orientation = CO_LEFT;
		if(goRight) player->orientation = CO_RIGHT;

		if(jump)
		{
			player->state = CS_JUMP;
			Game_UpdateDisplay(gd);
			jump = false;
		}
		else if(fall)
		{
			fallCpt++;
			if(fallCpt < 10) player->state = CS_DEFAULT;
			else             player->state = CS_SLIDE;

			Game_UpdateDisplay(gd);
			fall = false;
		}
		else if(walk)
		{
			walkCpt++;
			if(player->state == CS_DEFAULT && walkCpt == 5)
			{
				player->state = CS_WALK;
				walkCpt = 0;
			}
			else if((player->state == CS_WALK && walkCpt == 5) || resetDog)
			{
				player->state = CS_DEFAULT;
				walkCpt = 0;
				resetDog = false;
			}

			Game_UpdateDisplay(gd);
			walk = false;
		}
		else if(resetDog)
		{
			player->state = CS_DEFAULT;
			Game_UpdateDisplay(gd);
			resetDog = false;
		}

		SDL_Delay(25);
	}

	GameData_free(gd);

	return ERR_SUCCESS;
}