#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../utils.h"
#include "../config.h"
#include "../string-tools.h"
#include "../c-containers/list.h"

/**
 * Describe the possible state of a character
 */
typedef enum {
	CS_DEFAULT, ///< Default State
	CS_JUMP,    ///< In a jump
	CS_WALK,    ///< Used in walk animation
	CS_SLIDE,   ///< Slide and fall state
	CS_CRAWL_1, ///< Default in crawl mode (after touching a mob)
	CS_CRAWL_2  ///< Animation in crawl mode (after touching a mob)
} CharacterState;

/**
 * Describe the possible orientation of a character
 */
typedef enum {
	CO_LEFT,
	CO_RIGHT
} CharacterOrientation;

/**
 * The CharacterSurface structure store the surfaces for each state of a character.
 */
typedef struct _CharacterSurface {
	bool          loaded;    ///< Surface are loaded
	SDL_Rect      size;      ///< Size of a character surface

	SDL_Surface * def_r;     ///< Surface for CS_DEFAULT && Right
	SDL_Surface * jump_r;    ///< Surface for CS_JUMP    && Right
	SDL_Surface * walk_r;    ///< Surface for CS_WALK    && Right
	SDL_Surface * slide_r;   ///< Surface for CS_SLIDE   && Right
	SDL_Surface * crawl1_r;  ///< Surface for CS_CRAWL_1 && Right
	SDL_Surface * crawl2_r;  ///< Surface for CS_CRAWL_2 && Right

	SDL_Surface * def_l;     ///< Surface for CS_DEFAULT && Left
	SDL_Surface * jump_l;    ///< Surface for CS_JUMP    && Left
	SDL_Surface * walk_l;    ///< Surface for CS_WALK    && Left
	SDL_Surface * slide_l;   ///< Surface for CS_SLIDE   && Left
	SDL_Surface * crawl1_l;  ///< Surface for CS_CRAWL_1 && Left
	SDL_Surface * crawl2_l;  ///< Surface for CS_CRAWL_2 && Left
} CharacterSurface;

NEW_LIST_DEFINITION(CharacterList, CharacterSurface *, char *);

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

CharacterSurface * CharacterSurface_init(char * file);

#endif // __CHARACTER_H__