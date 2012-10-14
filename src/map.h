#ifndef _MAP_H_
#define _MAP_H_

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "SDL.h"
#include "SDL_image.h"
#include "list.h"
#include "gameUtilities.h"

#define BUFFER_SIZE 1024

#define BORDER 64

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Map{	

	// Map surface
	SDL_Surface *surfaceBack;
	SDL_Surface *surfaceFront;
	SDL_Surface *surfaceRefresh;

	// Map attributes
	int width, height;
	int tileWidth, tileHeight;
	
	xmlChar* layer;

	// Tileset
	SDL_Surface* tileSet;
	char *tileSetName, *tileSetImage;
	char *tileSetWidth, *tileSetHeight;
	char *firstgid;
	int  numTileSet;

	// Layer properties
	struct list_head listLayer;
	struct list_head listTileSet;
	char layerName;
	int  numLayers;
	
	int scroll_x, scroll_y, scrollVel;

	// Layers data
	int **collisions;
	int **charPosition;
	
} Map;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Map* MapConstructor(SDL_Surface *screen, char *file);
extern void MapDestructor(Map * map);
extern void MapUpdate(Map * map, SDL_Rect cursorCoords);
extern void MapDraw(Map *map, SDL_Surface* screen);

#endif // _MAP_H_
