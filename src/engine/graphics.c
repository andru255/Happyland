/**
 * @file   graphics.c
 * @author Baudouin Feildel
 * @brief  Source file of rendering engine
 */
#include "graphics.h"

int Graphics_Init()
{
	int SDL_FLAGS = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	int SDL_IMAGE_FLAGS = IMG_INIT_PNG;
	int SDL_MIXER_FLAGS = MIX_INIT_OGG;

	fprintf(stdout, "[PENDING] Initialize SDL"); fflush(stdout);
	if(SDL_Init(SDL_FLAGS) != 0)
	{
		fprintf(stdout, "\r[ERROR] Initialize SDL. See error log for more information.\n");
		fprintf(stderr, "Unable to initialize SDL : %s\n", SDL_GetError());

		return 1;
	}
	fprintf(stdout, "\r[OK] Initialize SDL.     \n");

	fprintf(stdout, "[PENDING] Initialize SDL Image"); fflush(stdout);
	if(IMG_Init(SDL_IMAGE_FLAGS) & SDL_IMAGE_FLAGS != SDL_IMAGE_FLAGS)
	{
		fprintf(stdout, "\r[ERROR] Initialize SDL Image. See error log for more information.\n");
		fprintf(stderr, "Unable to initialize SDL Image : %s\n", IMG_GetError());

		return 2;
	}
	fprintf(stdout, "\r[OK] Initialize SDL Image.     \n");

	fprintf(stdout, "[PENDING] Initialize SDL Mixer"); fflush(stdout);
	if(Mix_Init(SDL_MIXER_FLAGS) & SDL_MIXER_FLAGS != SDL_MIXER_FLAGS)
	{
		fprintf(stdout, "\r[ERROR] Initialize SDL Mixer. See error log for more information.\n");
		fprintf(stderr, "Unable to initialize SDL Mixer : %s\n", Mix_GetError());

		return 3;
	}

	if(Mix_OpenAudio(AUDIO_BITRATE, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		fprintf(stdout, "\r[ERROR] Initialize SDL Mixer. See error log for more information.\n");
		fprintf(stderr, "Unable to initialize SDL Mixer : %s\n", Mix_GetError());

		return 3;
	}
	fprintf(stdout, "\r[OK] Initialize SDL Mixer.     \n");
	return 0;
}

void Graphics_Close()
{
	// Close SDL Mixer
	fprintf(stdout, "[PENDING] Close SDL Mixer"); fflush(stdout);
	Mix_CloseAudio();
	Mix_Quit();
	fprintf(stdout, "\r[OK] Close SDL Mixer     \n");

	// Close SDL Image
	fprintf(stdout, "[PENDING] Close SDL Image"); fflush(stdout);
	IMG_Quit();
	fprintf(stdout, "\r[OK] Close SDL Image     \n");

	// Close SDL
	fprintf(stdout, "[PENDING] Close SDL"); fflush(stdout);
	SDL_Quit();
	fprintf(stdout, "\r[OK] Close SDL     \n");
}

void Graphics_createTileSurface(Asset * asset, Tile * tile)
{
	SDL_Rect blitRect;

	tile->surface = SDL_CreateRGBSurface(0, TILE_WIDTH, TILE_HEIGHT, 32, RMASK, GMASK, BMASK, AMASK);
	if(tile->surface == NULL)
	{
		fprintf(stderr, "createTileSurface.SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(ERR_CREATE_TILE_SURFACE);
	}

	Graphics_getTileRect(&blitRect, tile->posOnAsset);
	if(SDL_BlitSurface(asset->surface, &blitRect, tile->surface, NULL) != 0)
	{
		fprintf(stderr, "createTileSurface.SDL_BlitSurface failed: %s\n", SDL_GetError());
		exit(ERR_CREATE_TILE_SURFACE);
	}
}

void Graphics_getTileRect(SDL_Rect * out, SDL_Point position)
{
	out->x = position.x;
	out->y = position.y;
	out->w = TILE_WIDTH;
	out->h = TILE_HEIGHT;
}
