#ifndef __STRUCTURES_SRC_H__
#define __STRUCTURES_SRC_H__

#include "engine/structures.h"

typedef struct _Player {
	char                 * name;
	CharacterState         state;
	CharacterOrientation   orientation;
	SDL_Surface          * surface;
	CharacterSurface     * character;
	SDL_Rect               position;
	SDL_Point              speed;
	bool                   inJump;
	bool                   collision;
	int                    walkCpt;
} Player;

NEW_LIST_DEFINITION(PlayerList, Player *, char *);

typedef struct _GameData
{
	// Assets
	Asset_list * assets;

	// Maps
	Map_list * maps;
	Map      * currentMap;
	char     * currentMapName;

	// Characters
	CharacterList * characters;

	// Music
	Mix_Music * theme;
	bool isThemePlaying;

	// Players
	PlayerList * players;

	// SDL Window
	SDL_Window  * window;
	SDL_Surface * screen;

} GameData;

#endif // __STRUCTURES_SRC_H__