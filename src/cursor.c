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

#include "SDL.h"
#include "SDL_image.h"
#include "cursor.h"

Cursor* CursorConstructor(int x0, int y0)
{
	Cursor* cursor;
	cursor = (Cursor *)malloc(sizeof(Cursor));
	// Load sprites image
	if ((cursor->sprite = loadImage("data/Cursor.png")) == NULL)
		return NULL;
	
	// Set sprite initial position
	cursor->rcDest.x = x0 + 16;
	cursor->rcDest.y = y0 + 32;	

	cursor->free = 0;
	
	return cursor;
}

void CursorDestructor(Cursor *this)
{
	// Free character image
	SDL_FreeSurface(this->sprite);

	// Free character
	free(this);
}

void CursorDraw(Cursor* cursor, Map *map)
{
	SDL_BlitSurface(cursor->sprite, NULL, map->surfaceBack, &cursor->rcDest);
}

void CursorMove(Cursor* cursor, Orientation orientation)
{
	switch (orientation) {
		case ORIENT_WEST:
			if((((cursor->coordY-1) > -1) && (cursor->collisionArea[cursor->coordX][cursor->coordY - 1])) || (cursor->free)){
				cursor->rcDest.x = cursor->rcDest.x - (SPRITE_SIZE/2);
				if(!cursor->free)
					cursor->coordY--; 
			}
			break;
		case ORIENT_EAST:
			if((((cursor->coordY+1) < cursor->sideLength) && (cursor->collisionArea[cursor->coordX][cursor->coordY + 1])) || (cursor->free)){
				cursor->rcDest.x = cursor->rcDest.x + (SPRITE_SIZE/2);
				if(!cursor->free)
					cursor->coordY++;
			}
			break;
		case ORIENT_SOUTH:
			if((((cursor->coordX-1) > -1) && (cursor->collisionArea[cursor->coordX - 1][cursor->coordY])) || (cursor->free)){
				cursor->rcDest.y = cursor->rcDest.y + (SPRITE_SIZE/2);
				if(!cursor->free)
					cursor->coordX--;
			}
			break;
		case ORIENT_NORTH:
			if((((cursor->coordX+1) < cursor->sideLength) && (cursor->collisionArea[cursor->coordX + 1][cursor->coordY])) || (cursor->free)){
				cursor->rcDest.y = cursor->rcDest.y - (SPRITE_SIZE/2);
				if(!cursor->free)
					cursor->coordX++;
			}
			break;
		default:
			break;
	}	
}

void CursorSetPosition(Cursor *cursor, int x, int y)
{
	cursor->rcDest.x = x + 16;
	cursor->rcDest.y = y + 32;
}

void CursorSetCollisionArea(Cursor *cursor, int area)
{
	int i, j;
	int sizeSide = 2*area+1;

	// Set initial coord
	cursor->coordX = area;
	cursor->coordY = area;
	cursor->sideLength = sizeSide;	
		
	// Alloc move allowed area for the cursor
	cursor->collisionArea = (int **)malloc(sizeof(int*) * sizeSide);
	for(i = 0; i < sizeSide; i++)
		cursor->collisionArea[i] = (int *)malloc(sizeof(int) * sizeSide);
	
	// Set matrix
	for(i = 0; i < sizeSide; i++){
		for(j = 0; j < sizeSide; j++){
			if((j >= area - i) && (j <= area + i) && (i <= area))
				cursor->collisionArea[i][j] = 1;
			else if((j >= area - ((sizeSide-1) - i)) && (j <= area + ((sizeSide-1) - i)) && (i > area))
				cursor->collisionArea[i][j] = 1;
			else
				cursor->collisionArea[i][j] = 0;
			printf("%d ", cursor->collisionArea[i][j]);
		}
		printf("\n");
	}

/*
	// Free collision area
	for(i = 0; i < sizeSide; i++)
		free(cursor->collisionArea[i]);
	free(cursor->collisionArea);
*/
}
