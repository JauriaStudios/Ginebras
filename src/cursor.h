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

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"
#include "map.h"

struct Character;

typedef struct Cursor{

	SDL_Surface* sprite;
	SDL_Rect	 rcSrc;
	SDL_Rect     rcDest, rcPrevious; 
	int 		 **collisionArea;
	int 		 coordX, coordY;
	int 		 previousCoordX, previousCoordY; 
	int			 sideLength;
	int 		 free;
} Cursor;

extern Cursor* CursorConstructor(int x0, int y0);
extern void CursorDestructor(Cursor *this);
extern void CursorDraw(Cursor* cursor, Map *map);
extern void CursorMove(Cursor* cursor, Orientation orientation, Map *map);
extern void CursorSetPosition(Cursor *cursor, int x, int y);
extern void CursorSetCollisionArea(Cursor *cursor, int area);
extern int  CursorCheckEnemy(Cursor *cursor, Map *map, struct Character *character);
extern void CursorSetFree(Cursor *cursor);
extern void CursorSetNonFree(Cursor *cursor, struct Character *character);

#endif // _CURSOR_H_
