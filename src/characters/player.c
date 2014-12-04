#include "player.h"

IMPLEMENT_LIST(PlayerList,  Player *,  char *, Player_copy, Str_copy, Player_cmp, Str_cmp, Player_free, Str_free);

Player * Player_new(char * name)
{
	Player * tmp = NULL;
	tmp = calloc(sizeof(tmp), sizeof(Player));
	tmp->name = calloc(sizeof(char) * strlen(name), sizeof(char));
	tmp->state       = CS_DEFAULT;
	tmp->orientation = CO_RIGHT;
	tmp->surface     = NULL;
	tmp->character   = NULL;
	tmp->position.x  = 0;
	tmp->position.y  = 0;
	tmp->position.h  = 0;
	tmp->position.w  = 0;
	strcpy(tmp->name, name);
	return tmp;
}

void Player_free(Player * val)
{
	free(val);
}

int Player_cmp(Player * p1, Player * p2)
{
	return memcmp(p1, p2, sizeof(Player));
}

void Player_copy(Player ** dst, Player ** src)
{
	*dst = calloc(sizeof(Player), sizeof(Player));
	memcpy(*dst, *src, sizeof(Player));
}

void Player_GetBlitRect(Player * player, SDL_Rect * rect)
{
	rect->x = player->position.x;
	rect->y = player->position.y;
	rect->h = player->character->size.h;
	rect->w = player->character->size.w;
}

void Player_UpdateSurface(Player * player)
{
	player->surface = CharacterSurface_get(player->character, player->state, player->orientation);
}