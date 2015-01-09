#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "../utils.h"
#include "../c-containers/list.h"

/*
 * You need to update the TileShape_FromStr
 * when modifying this enumeration.
 */
typedef enum {
	TS_FULL              = 0, // Default
	TS_TRIANGLE_LEFT     = 1,
	TS_TRIANGLE_RIGHT    = 2,
	TS_MID_LEFT          = 3,
	TS_MID_RIGHT         = 4,
	TS_PLOT_TOP_LEFT     = 5,
	TS_PLOT_TOP_RIGHT    = 6,
	TS_PLOT_BOTTOM_LEFT  = 7,
	TS_PLOT_BOTTOM_RIGHT = 8
} TileShape;

/**
 * This struct contain information
 * about a tile
 */
typedef struct _Tile
{
	char        * name;        ///< Tile Name
	SDL_Point     posOnScreen; ///< Position of the tile on the screen
	SDL_Point     posOnAsset;  ///< Position of the tile on the asset
	bool          loaded;      ///< Is the surface loaded ?
	SDL_Surface * surface;     ///< SDL Surface
	TileShape     shape;       ///< Tile shape
} Tile;

NEW_LIST_DEFINITION(Tile_list,  Tile *,  int);


typedef struct _Asset
{
	char        * name;     ///< Asset Name
	SDL_Surface * surface;  ///< SDL_Surface
	Tile_list   * tiles;    ///< Tiles of the asset
} Asset;

NEW_LIST_DEFINITION(Asset_list, Asset *, int);

typedef enum {
	L_BG_SOFT  = 0,  // 0. Background not solid
	L_FG_SOFT  = 1,  // 1. Foreground not solid
	L_BG_SOLID = 2,  // 2. Background solid
	L_FG_SOLID = 3,  // 3. Foreground solid
	L_DEATH    = 4,  // 4. Death
	L_ICE      = 5,  // 5. Ice
	L_EXIT     = 6,  // 6. Exit
	L_DOOR     = 7,  // 7. Door
	L_SAFE     = 8,  // 8. Safe
	L_SWITCH   = 9,  // 9. Switch
	L_SWAP     = 10, // A. Swap
	L_START    = 11  // B. Start
} Layer;

/**
 * This struct contain information
 * about a map
 */
typedef struct _Map
{
	Tile        *** tiles;    ///< Tiles of the map
	Layer       **  layers;   ///< Layer identifier for each tile of the map
	SDL_Rect        clip;     ///< Rect displayed on the screen
	char        *   name;     ///< Name of the map
	char        *   filename; ///< Map filename
	SDL_Surface *   surface;  ///< SDL Surface
	bool            loaded;   ///< Flag that indicate if the map is loaded

	SDL_Point startPos;

	int   height;
	int   width;
	SDL_Color background_color;

	Tile_list * tileMap;

	struct {
		Layer left;
		Layer right;
		Layer top;
		Layer bottom;
	} edge;
} Map;

NEW_LIST_DEFINITION(Map_list, Map *, char *);

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

#endif // __STRUCTURES_H__