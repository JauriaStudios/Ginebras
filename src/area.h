#ifndef _AREA_H_
#define _AREA_H_

#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"
#include "map.h"

/**********************************************************
 **** STRUCTS 
 **********************************************************/
typedef struct Area{
	SDL_Surface* sprite;
	SDL_Rect     rcDest;
	int 		 radius;
} Area;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Area* AreaConstructor(int x0, int y0, int radius);
extern void AreaDestructor(Area *this);
extern void AreaDraw(Area *this, SDL_Surface* screen, Map *map);
extern void AreaSetPosition(Area *this, int x, int y);
#endif // _AREA_H_
