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
