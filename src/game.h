#ifndef __GAME_H__
#define __GAME_H__

#include "engine/graphics.h"

#include "characters/player.h"

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

GameData * GameData_new();

/*
 * Music functions
 */
/**
 * This function play the game theme
 * @param gd Game Data
 */
void Game_PlayTheme(GameData * gd);
/**
 * This function resume the game theme
 * @param gd Game Data
 */
void Game_ResumeTheme(GameData * gd);
/**
 * This function pause the game theme
 * @param gd Game Data
 */
void Game_PauseTheme(GameData * gd);
/**
 * This function toggle play/pause on the game theme
 * @param gd Game Data
 */
void Game_ToggleTheme(GameData * gd);

/*
 * Player function
 */
Player * Game_AddPlayer(GameData * gd, char * name, char * character);
Player * Game_GetPlayer(GameData * gd, char * name);
void Game_RemovePlayer(GameData * gd, char * name);
void Game_SetPlayerPositionOnMap(GameData * gd, char * playerName, int x, int y);

void GameData_free(GameData * gd);
void Game_LoadMap(GameData * gd, Map * map);
void Game_SelectMap(GameData * gd, char * mapName);
void Game_DisplayMap(GameData * gd, Map * map, bool updateWindow);
void Game_UpdateDisplay(GameData * gd);

#endif // __GAME_H__