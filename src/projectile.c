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

#include "projectile.h"
#include "characters.h"

Projectile* ProjectileConstructor(char *fileSprite)
{
	// Variable definition section
	Projectile *this;

	// Alloc
	this = (Projectile*) malloc(sizeof(Projectile));

	// Load sprite	
	if (!(this->sprite = loadImage(fileSprite))){
		printf("ProjectileConstructor ERROR: couldn't load %s \n", fileSprite);
		return NULL;
	}

	this->velocity	 = SPEED_ARROW;
	this->actualStep = 0;
	this->moveSteps  = 0;
	this->moving	 = 0;
	this->action 	 = 0;

	this->rcSrc.w = PROJECTILE_TILE_SIZE;
	this->rcSrc.h = PROJECTILE_TILE_SIZE;

	return this;
}

void ProjectileDestructor(Projectile *this)
{
	free(this);
}

void ProjectileSetTrajectory(Projectile *this, Character *character, int x, int y)
{
	float m, b; 
	int i, dx, dy;
	int stepsX, stepsY;
	int coordX = 0, coordY = 0;
	
	// Set status move
	this->moving     = 1;
	this->actualStep = 0;
	this->moveState  = 1;
	this->moveSteps  = 0;
	this->skipFrames = 0;
	this->collision  = 0;
	this->action	 = 0;
	
	
	switch(character->moveOrient){
		case ORIENT_NORTH:
			coordX = character->rcDest.x + ARROW_OFFSET_NORTH_X; 
			coordY = character->rcDest.y + ARROW_OFFSET_NORTH_Y;
			x += ARROW_OFFSET_NORTH_X;
			y += ARROW_OFFSET_NORTH_Y;
			break;
		case ORIENT_WEST:
			coordX = character->rcDest.x + ARROW_OFFSET_WEST_X; 
			coordY = character->rcDest.y + ARROW_OFFSET_WEST_Y;
			x += ARROW_OFFSET_WEST_X;
			y += ARROW_OFFSET_WEST_Y;
			break;
		case ORIENT_SOUTH:
			coordX = character->rcDest.x + ARROW_OFFSET_SOUTH_X; 
			coordY = character->rcDest.y + ARROW_OFFSET_SOUTH_Y;
			x += ARROW_OFFSET_SOUTH_X;
			y += ARROW_OFFSET_SOUTH_Y;
			break;
		case ORIENT_EAST:
			coordX = character->rcDest.x + ARROW_OFFSET_EAST_X; 
			coordY = character->rcDest.y + ARROW_OFFSET_EAST_Y;
			x += ARROW_OFFSET_EAST_X;
			y += ARROW_OFFSET_EAST_Y;
			break;
		default:
			break;
	}

	// Set destination point
	this->rcDest.x = coordX;
	this->rcDest.y = coordY;

	//  Slope of the line
	dx = x - coordX;
	dy = y - coordY;

	// Fix the offset

	// skip infinite slope for the vertical movement
	if(dx)
		m = (float) dy/dx;
	else
		m = 0.0;

	// b of the line
	b = (coordY) - (m*coordX);

	// steps
	stepsX = abs( dx ) / this->velocity;
	stepsY = abs( dy ) / this->velocity;
	
	this->moveSteps = ceil(sqrt((stepsX*stepsX) + (stepsY*stepsY)));
	
	// Set orientation movement
	this->moveOrient = character->moveOrient;

	// Initialitate move vectors
	this->moveX = (int *)malloc(sizeof(int)*this->moveSteps);
	this->moveY = (int *)malloc(sizeof(int)*this->moveSteps);

	// Set move vectors
	if(dx){
		for(i=0; i < this->moveSteps-1; i++){
			this->moveX[i] = this->rcDest.x + ( (i *  dx ) / this->moveSteps ) ;			
			this->moveY[i] = m*this->moveX[i] + b; 
		}
		this->moveX[this->moveSteps-1] = x;
		this->moveY[this->moveSteps-1] = y;
	}else{
		for(i=0; i < this->moveSteps-1; i++){
			this->moveX[i] = this->rcDest.x;			
			this->moveY[i] = this->rcDest.y + ( (i *  dy ) / this->moveSteps ) ;	
		}
		this->moveX[this->moveSteps-1] = x;
		this->moveY[this->moveSteps-1] = y;
	}
}

void ProjectileDraw(Projectile *this, Map *map)
{
	if(this->actualStep < this->moveSteps && this->moving){
		SDL_BlitSurface(this->sprite, &this->rcSrc, map->surfaceBack, &this->rcDest);	
		this->rcDest.x = this->moveX[this->actualStep];
		this->rcDest.y = this->moveY[this->actualStep];
		this->actualStep++;
	}else if(this->actualStep == this->moveSteps) {
		//this->action = 0;
	}else {
		this->moving = 0;
	}
}
