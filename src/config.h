#ifndef __CONFIG_H__
#define __CONFIG_H__

#define ASSETS_DIR         "./assets/graphics"
#define ASSETS_CONFIG_FILE "./assets/graphics/assets.conf"

#define MAPS_DIR "./assets/maps"


// Don't modify anithing below this line
// =====================================


#define TILE_HEIGHT 32
#define TILE_WIDTH  32

#define ASSETS_DIR_LEN sizeof(ASSETS_DIR)
#define MAPS_DIR_LEN   sizeof(MAPS_DIR)

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

#endif // __CONFIG_H__