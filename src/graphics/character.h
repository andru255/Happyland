#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../utils.h"
#include "../config.h"
#include "../string-tools.h"

/**
 * The CharacterState enumeration describe the possible state of a character
 */
typedef enum {
	CS_DEFAULT,
	CS_JUMP,
	CS_WALK,
	CS_SLIDE,
	CS_CRAWL_1,
	CS_CRAWL_2
} CharacterState;

/**
 * The CharacterSurface structure store the surfaces for each state of a character.
 */
typedef struct _CharacterSurface {
	bool          loaded;  ///< Surface are loaded
	SDL_Rect      size;    ///< Size of a character surface
	SDL_Surface * def;     ///< Surface for CS_DEFAULT
	SDL_Surface * jump;    ///< Surface for CS_JUMP
	SDL_Surface * walk;    ///< Surface for CS_WALK
	SDL_Surface * slide;   ///< Surface for CS_SLIDE
	SDL_Surface * crawl1;  ///< Surface for CS_CRAWL_1
	SDL_Surface * crawl2;  ///< Surface for CS_CRAWL_2
} CharacterSurface;

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
 * This function gets a SDL_Surface from a CharacterSurface depending on state
 * @param  cs    The CharacterSurface to get from
 * @param  state The state to get
 * @return       The extracted SDL_Surface
 */
SDL_Surface * CharacterSurface_get(CharacterSurface * cs, CharacterState state);

/**
 * Load all surface in a CharacterSurface object
 * @param cs   The CharacterSurface where to load surface
 * @param file The definition file
 */
bool CharacterSurface_load(CharacterSurface * cs, char * file);

#endif // __CHARACTER_H__