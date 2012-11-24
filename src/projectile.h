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

#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"
#include "map.h"

#define PROJECTILE_TILE_SIZE   64

#define ARROW_OFFSET_NORTH_X (+0)
#define ARROW_OFFSET_NORTH_Y (-32)

#define ARROW_OFFSET_WEST_X  (-32)
#define ARROW_OFFSET_WEST_Y  (-8)

#define ARROW_OFFSET_SOUTH_X (+0)
#define ARROW_OFFSET_SOUTH_Y (+8)

#define ARROW_OFFSET_EAST_X  (+32)
#define ARROW_OFFSET_EAST_Y  (-8)

#define SPEED_ARROW			   4

/**********************************************************
 *** STRUCTS
 **********************************************************/
struct Character;

typedef struct Projectile{
	SDL_Surface *sprite;
	SDL_Rect	rcDest, rcSrc;

	// Set trajectory
	int moving;    
	int actualStep;
	int moveState; 
	int moveSteps; 
	int skipFrames;
	int collision;

	// Destination point
	int xDest;
	int yDest;

	// Movement management
	int velocity;
	int* moveY;
	int* moveX;
	Orientation moveOrient;

	int action;
} Projectile;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Projectile* ProjectileConstructor(char *fileSprite);
extern void ProjectileDestructor(Projectile *this);
extern void ProjectileSetTrajectory(Projectile *this, struct Character *character, int x, int y);
extern void ProjectileDraw(Projectile *this, Map *map);

#endif // _PROJECTILE_H_
