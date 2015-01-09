#ifndef __CONFIG_H__
#define __CONFIG_H__

#define ASSETS_DIR         "./assets/graphics"
#define ASSETS_CONFIG_FILE "./assets/graphics/assets.conf"

#define TILE_HEIGHT 32
#define TILE_WIDTH  32

#define MAPS_DIR       "./assets/maps"
#define MAPS_LIST_FILE "./assets/maps/map.list"

#define CHARACTER_DIR       "./assets/characters"
#define CHARACTER_LIST_FILE "./assets/characters/character.list"

#define AUDIO_BITRATE 22050

#define GRAVITY 20

//#define __DEBUG__
//#define __DEBUG_LVL_0__
//#define __DEBUG_LVL_1__
//#define __DEBUG_LVL_2__
//#define __DEBUG_LVL_3__


// Don't modify anithing below this line
// =====================================

#define ASSETS_DIR_LEN    sizeof(ASSETS_DIR)
#define MAPS_DIR_LEN      sizeof(MAPS_DIR)
#define CHARACTER_DIR_LEN sizeof(CHARACTER_DIR)

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define RMASK (0xff000000)
    #define GMASK (0x00ff0000)
    #define BMASK (0x0000ff00)
    #define AMASK (0x000000ff)
#else
    #define RMASK (0x000000ff)
    #define GMASK (0x0000ff00)
    #define BMASK (0x00ff0000)
    #define AMASK (0xff000000)
#endif

#if defined(__DEBUG__)

	#if defined(__DEBUG_LVL_3__)
		#define __DEBUG_LVL_0__
		#define __DEBUG_LVL_1__
		#define __DEBUG_LVL_2__

		#define __DEBUG_PARSING_SHOW_LINES__
	#endif

	#if defined(__DEBUG_LVL_2__)
		#define __DEBUG_LVL_0__
		#define __DEBUG_LVL_1__

		#define __DEBUG_COLLISION__
	#endif

	#if defined(__DEBUG_LVL_1__)
		#define __DEBUG_LVL_0__

		#define __DEBUG_MAP_LOAD__
	#endif

	#if defined(__DEBUG_LVL_0__)
		#define __DEBUG_MAP_FN__
		#define __DEBUG_ASSET_LOAD__
	#endif

#endif

#endif // __CONFIG_H__