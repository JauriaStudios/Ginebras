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
