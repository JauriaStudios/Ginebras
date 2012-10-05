#ifndef _MAP_H_
#define _MAP_H_

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "SDL.h"
#include "SDL_image.h"
#include "list.h"
#include "gameUtilities.h"

//#include "cursor.h"

#define SCREEN_WIDTH  	800
#define SCREEN_HEIGHT 	600

#define BUFFER_SIZE 1024

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Map{	

	// Map surface
	SDL_Surface *surfaceBack;
	SDL_Surface *surfaceFront;
	SDL_Surface *surfaceRefresh;

	xmlChar* layer;

	// Tileset
	SDL_Surface* tileSet;
	char *tileSetName, *tileSetImage;
	char *tileSetWidth, *tileSetHeight, *tileWidth, *tileHeight;
	char *firstgid;
	int  numTileSet;

	// Layer properties
	struct list_head listLayer;
	struct list_head listTileSet;
	char layerName;
	int  numLayers;
	
	int scroll_x, scroll_y, scrollVel;

	// Layers data
	int collisions[MAP_SIZE_X][MAP_SIZE_Y];
	int **charPosition;
	
} Map;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Map* MapConstructor(SDL_Surface *screen);
extern void MapDestructor(Map * map);
extern void MapLoad(Map * map, char* file, SDL_Surface *screen);
extern void MapUpdate(Map * map, SDL_Rect cursorCoords);
extern void MapDraw(Map *map, SDL_Surface* screen);

#endif // _MAP_H_
