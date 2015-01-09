#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <assert.h>

#include "../config.h"
#include "../engine/graphics.h"
#include "../structures.h"

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


void Player_SetCharacter(Player * player, CharacterSurface * cs);

void Player_UpdateSurface(Player * player, Map * map);

void Player_ResetSpeed(Player * player);

void Player_ApplyGravity(Player * player, int gravity);

bool Player_HaveColision(Player * player, Map * map);

#endif // __PLAYER_H__