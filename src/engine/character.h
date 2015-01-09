#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../utils.h"
#include "../config.h"
#include "../string-tools.h"
#include "../c-containers/list.h"

#include "structures.h"

/**
 * This function initialize a CharacterSurface structure
 * @return An initialized CharacterSurface structure
 */
CharacterSurface * CharacterSurface_new();

/**
 * This function free a CharacterSurface structure
 * @param val CharacterSurface object to free
 */
void CharacterSurface_free(CharacterSurface * val);

/**
 * Compare two character surface
 * @param  val1 The first struct
 * @param  val2 The second struct
 * @return      0 if equal, 1 if val1 > val2, -1 if val1 < val2
 */
int CharacterSurface_cmp(CharacterSurface * val1, CharacterSurface * val2);

/**
 * Copy two a CharacterSurface
 * @param dest Destination
 * @param src  Source
 */
void CharacterSurface_copy(CharacterSurface ** dest, CharacterSurface ** src);

/**
 * This function gets a SDL_Surface from a CharacterSurface depending on state
 * @param  cs          The CharacterSurface to get from
 * @param  state       The state of the character
 * @param  orientation The orientation of the character
 * @return             The extracted SDL_Surface
 */
SDL_Surface * CharacterSurface_get(CharacterSurface * cs, CharacterState state, CharacterOrientation orientation);

/**
 * Load all surface in a CharacterSurface object
 * @param cs   The CharacterSurface where to load surface
 * @param file The definition file
 */
bool CharacterSurface_load(CharacterSurface * cs, char * file);

/**
 * Load the list of available characters
 * @return A character list
 */
CharacterList * CharacterSurface_LoadList();

/**
 * Initialize a CharacterSurface from a file
 * @param  file Filename to use
 * @return      An initialized CharacterSurface object
 */
CharacterSurface * CharacterSurface_init(char * file);

#endif // __CHARACTER_H__