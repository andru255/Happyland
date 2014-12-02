#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../config.h"
#include "../graphics/graphics.h"

typedef struct _Player {
	CharacterState   state;
	SDL_Surface    * surface;
} Player;

Player * Player_new();
void Player_free(Player * val);

#endif // __PLAYER_H__