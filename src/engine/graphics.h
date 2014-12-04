/**
 * @file   graphics.h
 * @author Baudouin Feildel
 * @brief  Header file for rendering engine
 */
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "tile.h"
#include "asset.h"
#include "map.h"
#include "character.h"

#include "../error-codes.h"
#include "../config.h"
#include "../utils.h"
#include "../string-tools.h"

/*
 * General Graphics functions
 */
/**
 * Initialize the graphics stack
 * @return 0 if initialization was ok
 */
int Graphics_Init();

/**
 * Close the graphics stack
 */
void Graphics_Close();

/**
 * Create a SDL_Surface, load the data
 * and store it in Tile struct
 * @param asset    The asset which contain the tile
 * @param tile     The tile where to store the SDL_Surface
 * @param position The position of the tile in the asset
 */
void Graphics_createTileSurface(Asset * asset, Tile * tile);

/**
 * Create a Tile rectangle from a SDL_Point
 * @param out      Output value
 * @param position Origin of the rect
 */
void Graphics_getTileRect(SDL_Rect * out, SDL_Point position);

#endif // __GRAPHICS_H__