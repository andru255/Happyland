#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../config.h"
#include "../engine/graphics.h"

typedef struct _Player {
	char                 * name;
	CharacterState         state;
	CharacterOrientation   orientation;
	SDL_Surface          * surface;
	CharacterSurface     * character;
	SDL_Rect               position;
} Player;

NEW_LIST_DEFINITION(PlayerList, Player *, char *);

/**
 * Create a Player object
 * @return The new Player object
 */
Player * Player_new(char * name);

/**
 * Delete a Player object
 * @param val The object to delete
 */
void Player_free(Player * val);

/**
 * Compare two player (memory comparison)
 * @param  p1 Player 1
 * @param  p2 Player 2
 * @return    0 if p1 == p2 ; 1 if p1 > p2 ; else -1
 */
int Player_cmp(Player * p1, Player * p2);

/**
 * Copy a Player object
 * @param dst Destination
 * @param src Source
 */
void Player_copy(Player ** dst, Player ** src);

void Player_GetBlitRect(Player * player, SDL_Rect * rect);

void Player_UpdateSurface(Player * player);

#endif // __PLAYER_H__