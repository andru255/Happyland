#include "player.h"

Player * Player_new()
{
	Player * tmp = NULL;
	tmp = calloc(sizeof(tmp), sizeof(tmp));
	tmp->state   = CS_DEFAULT;
	tmp->surface = NULL;
	return tmp;
}

void Player_free(Player * val)
{
	free(val);
}