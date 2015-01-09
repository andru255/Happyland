#include "player.h"

IMPLEMENT_LIST(PlayerList,  Player *,  char *, Player_copy, Str_copy, Player_cmp, Str_cmp, Player_free, Str_free);

Player * Player_new(char * name)
{
	Player * tmp = NULL;
	assert(name != NULL);
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
	tmp->speed.x     = 0;
	tmp->speed.y     = 0;
	tmp->inJump      = false;
	tmp->collision   = false;
	tmp->walkCpt     = 0;
	strcpy(tmp->name, name);
	return tmp;
}

void Player_free(Player * val)
{
	if(val != NULL)
	{
		free(val->name);
		free(val);
	}
}

int Player_cmp(Player * p1, Player * p2)
{
	assert(p1 != NULL);
	assert(p2 != NULL);
	return memcmp(p1, p2, sizeof(Player));
}

void Player_copy(Player ** dst, Player ** src)
{
	assert(*src != NULL);
	*dst = calloc(sizeof(Player), sizeof(Player));
	memcpy(*dst, *src, sizeof(Player));
}

void Player_SetCharacter(Player * player, CharacterSurface * cs)
{
	assert(player != NULL);
	assert(cs != NULL);
	player->character = cs;
	player->position.h = player->character->size.h;
	player->position.w = player->character->size.w;
	player->surface    = CharacterSurface_get(player->character, player->state, player->orientation);
}

bool Player_HaveColision(Player * player, Map * map)
{
	SDL_Point pt[6];
	Layer points[6] = {L_BG_SOFT, L_BG_SOFT, L_BG_SOFT, L_BG_SOFT, L_BG_SOFT, L_BG_SOFT};

	assert(player != NULL && map != NULL);

	// Top left
	pt[0].x = player->position.x;
	pt[0].y = player->position.y;

	// Top Right
	pt[1].x = player->position.x + player->position.w - 1;
	pt[1].y = player->position.y;

	// Bottom right
	pt[2].x = player->position.x + player->position.w - 1;
	pt[2].y = player->position.y + player->position.h - 1;

	// Bottom left
	pt[3].x = player->position.x;
	pt[3].y = player->position.y + player->position.h - 1;

	// Middle left
	pt[4].x = player->position.x;
	pt[4].y = player->position.y + player->position.h / 2;

	// Middle Right
	pt[5].x = player->position.x + player->position.w - 1;
	pt[5].y = player->position.y + player->position.h / 2;

	points[0] = Map_GetPixelLayer(map, pt[0].x, pt[0].y);
	points[1] = Map_GetPixelLayer(map, pt[1].x, pt[1].y);
	points[2] = Map_GetPixelLayer(map, pt[2].x, pt[2].y);
	points[3] = Map_GetPixelLayer(map, pt[3].x, pt[3].y);
	points[4] = Map_GetPixelLayer(map, pt[4].x, pt[4].y);
	points[5] = Map_GetPixelLayer(map, pt[5].x, pt[5].y);

	#ifdef __DEBUG_COLLISION__
		fprintf(stdout, "\rCollision: top-lef[%3d,%3d]:%s top-rig[%3d,%3d]:%s bot-rig[%3d,%3d]:%s bot-lef[%3d,%3d]:%s mid-left[%3d,%3d]:%s mid-rig[%3d,%3d]:%s",
			pt[0].x, pt[0].y, Layer_GetName(points[0]),
			pt[1].x, pt[1].y, Layer_GetName(points[1]),
			pt[2].x, pt[2].y, Layer_GetName(points[2]),
			pt[3].x, pt[3].y, Layer_GetName(points[3]),
			pt[4].x, pt[4].y, Layer_GetName(points[4]),
			pt[5].x, pt[5].y, Layer_GetName(points[5])
		);
	#endif

	return Layer_IsSolid(points[0]) || Layer_IsSolid(points[1]) || Layer_IsSolid(points[2]) || Layer_IsSolid(points[3]) || Layer_IsSolid(points[4]) || Layer_IsSolid(points[5]);
}

void Player_UpdateSurface(Player * player, Map * map)
{
	SDL_Rect target      = {0,0,0,0};
	bool     endmove     = false;
	bool     colision    = false;
	bool     colisionOnX = false;
	bool     colisionOnY = false;


	assert(player != NULL);

	// Update state

	if(player->speed.x == 0 && player->speed.y == 0)
		player->state = CS_DEFAULT;
	else
	{
		if(player->speed.x != 0)
		{
			player->walkCpt++;
			if(player->state == CS_DEFAULT && player->walkCpt == 7)
			{
				player->state = CS_WALK;
				player->walkCpt = 0;
			}
			else if(player->state == CS_WALK && player->walkCpt == 7)
			{
				player->state = CS_DEFAULT;
				player->walkCpt = 0;
			}

			if(player->speed.x > 0)
				player->orientation = CO_RIGHT;
			else if(player->speed.x < 0)
				player->orientation = CO_LEFT;
		}

		if(player->speed.y < 0)
		{
			player->state = CS_JUMP;
		}
		else if(player->speed.y > 0)
		{
			player->state = CS_SLIDE;
		}
	}

	player->surface  = CharacterSurface_get(player->character, player->state, player->orientation);

	// If a map is open check for collision
	if(map != NULL)
	{
		// Set move target
		target.x = player->position.x + player->speed.x;
		target.y = player->position.y + player->speed.y;

		endmove = false;
		while(!endmove && (player->speed.x != 0 || player->speed.y != 0))
		{
			// Move
			if(player->position.x != target.x && !colisionOnX)
			{
				     if(player->speed.x > 0) player->position.x++;
				else if(player->speed.x < 0) player->position.x--;
			}

			if(player->position.y != target.y && !colisionOnY)
			{
				     if(player->speed.y > 0) player->position.y++;
				else if(player->speed.y < 0) player->position.y--;
			}

			if(Player_HaveColision(player, map))
			{
				// Go backward in no collision place
				     if(player->speed.x > 0) player->position.x--;
				else if(player->speed.x < 0) player->position.x++;

				     if(player->speed.y > 0) player->position.y--;
				else if(player->speed.y < 0) player->position.y++;

				// Find collision axis
				if(player->position.x != target.x && !colisionOnX)
				{
					     if(player->speed.x > 0) player->position.x++;
					else if(player->speed.x < 0) player->position.x--;

					if(Player_HaveColision(player, map))
					{
						colisionOnX = true;
						     if(player->speed.x > 0) player->position.x--;
						else if(player->speed.x < 0) player->position.x++;
					}
				}

				if(player->position.y != target.y && !colisionOnY)
				{
					     if(player->speed.y > 0) player->position.y++;
					else if(player->speed.y < 0) player->position.y--;

					if(Player_HaveColision(player, map))
					{
						colisionOnY = true;
						     if(player->speed.y > 0) player->position.y--;
						else if(player->speed.y < 0) player->position.y++;
					}
				}

				if(colisionOnX && colisionOnY)
					endmove = true;
			}

			if((player->position.x == target.x && player->position.y == target.y) ||
			   (colisionOnY && colisionOnX) ||
			   (colisionOnY && player->position.x == target.x) ||
			   (colisionOnX && player->position.y == target.y) ||
			   (colisionOnX && player->position.x == target.x) ||
			   (colisionOnY && player->position.y == target.y)
			   )
				endmove = true;
		}

		// Apply Gravity if needed
		if(player->speed.y == 0 && !player->inJump || colision)
		{
			// Test if player is on flat
			player->position.y++;

			if(Player_HaveColision(player, map))
				player->position.y--;
			else
			{
				endmove = false;
				target.y = player->position.y + 20;
				while(!endmove)
				{
					if(player->position.y != target.y)
						player->position.y++;

					if(Player_HaveColision(player, map))
					{
						endmove = true;
						player->position.y--;
					}

					if(player->position.y == target.y)
						endmove = true;
				}
			}
		}
	}
	// If no map open just move player
	else
	{
		// Apply Speed
		player->position.x += player->speed.x;
		player->position.y += player->speed.y;

		// Apply Gravity
		player->position.y += GRAVITY;
	}

	Player_ResetSpeed(player);
}

void Player_ResetSpeed(Player * player)
{
	assert(player != NULL);
	player->speed.x = 0;
	player->speed.y = 0;
}

void Player_ApplyGravity(Player * player, int gravity)
{
	assert(player != NULL);
	if(!player->inJump)
		player->speed.y = gravity;
}

void Player_AddOverlay(Player * player, SDL_Surface * screen)
{
	static SDL_Color color = {0x00, 0x00, 0x00, 0x50};
	SDL_Surface * overlay = SDL_CreateRGBSurface(0, player->character->size.w, player->character->size.h, 32, RMASK, GMASK, BMASK, AMASK);

	SDL_FillRect(overlay, NULL,
		SDL_MapRGBA(
			overlay->format,
			color.r,
			color.g,
			color.b,
			color.a
		)
	);

	SDL_BlitSurface(overlay, NULL, screen, &(player->position));
	SDL_FreeSurface(overlay);
}