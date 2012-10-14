#ifndef _GRID_H_
#define _GRID_H_

#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"
#include "map.h"

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Grid{
	SDL_Surface *tile;
	SDL_Surface *image;
} Grid;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Grid* GridConstructor(int sizeX, int sizeY);
extern void GridDraw(Grid *this, Map *map);
extern void GridDestructor(Grid *this);

#endif // _GRID_H_
