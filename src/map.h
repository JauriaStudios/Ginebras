/* Copyright (C) 2012 Jauria Studios
 *
 * Sons of Battle is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Sons of Battle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Sons of battle. if not, see <http://www.gnu.org/licenses/>.
 */

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
	int ***charPosition; // charPosition index 0 p1+p2, 1 p1, 2 p2
	
} Map;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Map* MapConstructor(SDL_Surface *screen, char *file);
extern void MapDestructor(Map * map);
extern void MapUpdate(Map * map, SDL_Rect cursorCoords);
extern void MapDraw(Map *map, SDL_Surface* screen);

#endif // _MAP_H_
