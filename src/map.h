#ifndef _MAP_H_
#define _MAP_H_

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"

//#include "cursor.h"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH  	800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 	600
#endif

#define BUFFER_SIZE 1024

#define MAP_SIZE_X 40
#define MAP_SIZE_Y 40

#ifndef TILE_SIZE
#define TILE_SIZE 32

#endif

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Map{	

	SDL_Surface*	surfaceBackground[MAP_SIZE_X][MAP_SIZE_Y];
	SDL_Surface*	tileSet;
	SDL_Rect	rcGrassSrc, rcGrassDest;
	
	xmlChar* layer;

	// tileset
	char		*tileSetName, *tileSetImage;
	char		*tileSetWidth, *tileSetHeight, *tileWidth, *tileHeight;
	char		*firstgid;
	int		numTileSet;

	
	char		layerName;
	int		numLayers;
	
	int		scroll_x, scroll_y, scrollVel, mapWidth, mapHeight;

	int background[MAP_SIZE_X][MAP_SIZE_Y];
	//int objects[MAP_SIZE_X][MAP_SIZE_Y];
	//int collisions[MAP_SIZE_X][MAP_SIZE_Y];
	
} Map;


/**********************************************************
 *** METHODS
 **********************************************************/
extern Map* MapConstructor();
extern void MapLoad(Map * map, char* file);
extern void MapUpdate(Map * map, SDL_Rect cursorCoords);
extern void MapDraw(Map * map, SDL_Surface* screen);
extern void MapDestructor(Map * map);

#endif

