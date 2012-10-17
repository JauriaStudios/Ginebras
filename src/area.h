#ifndef _AREA_H_
#define _AREA_H_

#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"
#include "map.h"
#include "cursor.h"

/**********************************************************
 **** STRUCTS 
 **********************************************************/
typedef struct Area{
	SDL_Surface *tile; 
	SDL_Rect     rcDest;
	int 		 radius;
} Area;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Area* AreaConstructor(int x0, int y0, int radius);
extern void AreaDestructor(Area *this);
extern void AreaSetPosition(Area *this, int x, int y);
extern void AreaDraw(Area *this, Map *map, Cursor *cursor);

#endif // _AREA_H_
