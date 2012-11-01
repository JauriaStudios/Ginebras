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

#include "characters.h"

typedef enum OrientCollision {
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_WEST,
	SOUTH_EAST,
} OrientCollision;

Character* CharacterConstructor(char* file, Orientation or, int x0, int y0, int iniciative, int movement, int **pos)
{
	Character* character;
	character = (Character *)malloc(sizeof(Character));
	char spriteMove[75];
	char spriteSlash[75];
	char sprite192Slash[75];
	char spriteSpell[75];
	int coordX, coordY;

	sprintf(spriteMove, "%sWalk.png", file);
	// Load movement sprites
	if (!(character->sprite = loadImage(spriteMove))){
		printf("Character constructor ERROR: couldn't load character Walk sprite\n");
		return NULL;
	}
	
	sprintf(spriteSlash, "%sSlash.png", file);
	// Load slash sprites
	if (!(character->spriteSlash = loadImage(spriteSlash))){
		SDL_FreeSurface(character->sprite);
		printf("Character constructor ERROR: couldn't load character Slash sprite\n");
		return NULL;
	}

	sprintf(sprite192Slash, "%s192Slash.png", file);
	// Load 192 slash sprite 	
	if (!(character->sprite192Slash = loadImage(sprite192Slash))){
		SDL_FreeSurface(character->sprite);
		printf("Character constructor ERROR: couldn't load character Slash192 sprite\n");
		return NULL;
	}

	sprintf(spriteSpell, "%sSpell.png", file);
	// Load spell sprite 	
	if (!(character->spriteSpell = loadImage(spriteSpell))){
		SDL_FreeSurface(character->sprite);
		printf("Character constructor ERROR: couldn't load character Spell sprite\n");
		return NULL;
	}

	// Set movement
	character->movement = movement;
	if (!(character->moveArea = AreaConstructor(x0, y0, movement))){
		SDL_FreeSurface(character->sprite);
		SDL_FreeSurface(character->spriteSlash);
		printf("Character constructor ERROR: couldn't create movement area\n");
		return NULL;
	}
	
    // Set initial state
    character->state = MOVING;

	// Set sprite initial position
	character->rcDest.x = x0;
	character->rcDest.y = y0;

	// Set initial animation sprite frame
	character->rcSrc.x = 0;
	character->rcSrc.y = or * SPRITE_SIZE;
	character->rcSrc.w = SPRITE_SIZE;
	character->rcSrc.h = SPRITE_SIZE;
	
	// Set initial character position
	GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);
	pos[coordY][coordX] = 1;

	// Set initial orientation
	character->moveOrient = or;
		
	// Set initial velocity
	character->velocity = 5;

	// Set initial destination point
	character->destinationPoint.x = x0;
	character->destinationPoint.y = y0;
	
	// Set initial state
	character->moving = 0;
	character->actualStep = 0;
	character->moveSteps  = 0;
	character->moveState = 0;

	// Set linked list
	INIT_LIST_HEAD(&character->list);
	INIT_LIST_HEAD(&character->listSort);

	// Set initial turn variables
	character->check = 0;
	character->iniciative = iniciative;

	// Set initial attack variables
	character->rcSrcAttack.x = 0;
	character->rcSrcAttack.y = or * SPRITE_SIZE;
	character->rcSrcAttack.w = SPRITE_SIZE;
	character->rcSrcAttack.h = SPRITE_SIZE;
	
	character->attackState = 0;
	character->attacking = 0;
	character->actualAttackStep = 0;

	return character;
}

void CharacterDestructor(Character *character)
{
	// Free character images
	SDL_FreeSurface(character->sprite);
	SDL_FreeSurface(character->spriteSlash);
	SDL_FreeSurface(character->sprite192Slash);
	SDL_FreeSurface(character->spriteSpell);
	
	// Free movement area
	AreaDestructor(character->moveArea);
	
	// Free character
	free(character);
}

void CharacterSetDestination(Character* character, Cursor* cursor, Map *map)
{
	float m, b; 
	int i, dx, dy;
	int stepsX, stepsY;
	int coordX, coordY;
	
	// Set status move
	character->moving     = 1;
	character->actualStep = 0;
	character->moveState  = 1;
	character->moveSteps  = 0;
	character->skipFrames = 0;
	character->collision  = 0;
	
	// Set to 0 character position
	GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);
	map->charPosition[coordY][coordX] = 0;

	// Set destination point
	character->destinationPoint.x = cursor->rcDest.x - 16;
	character->destinationPoint.y = cursor->rcDest.y - 32;
	
	//  Slope of the line
	dx = character->destinationPoint.x - character->rcDest.x;
	dy = character->destinationPoint.y - character->rcDest.y;
	
	// skip infinite slope for the vertical movement
	if(dx)
		m = (float) dy/dx;
	else
		m = 0.0;

	// b of the line
	b = character->rcDest.y - ( m*character->rcDest.x );
	
	// steps
	stepsX = abs( dx ) / character->velocity;
	stepsY = abs( dy ) / character->velocity;
	
	character->moveSteps = ceil(sqrt( (stepsX*stepsX) + (stepsY*stepsY) ));
	
	// Set orientation movement
	if( (dy < 0) && (stepsY > stepsX) )
		character->moveOrient = ORIENT_NORTH;
	else if( (dy > 0) && ( stepsY > stepsX ) )
		character->moveOrient = ORIENT_SOUTH;
	else if( (dx > 0) && ( stepsX >= stepsY) )
		character->moveOrient = ORIENT_EAST;
	else if( (dx < 0) && ( stepsX >= stepsY ) )
		character->moveOrient = ORIENT_WEST;

	// Initialitate move vectors
	character->moveX = (int *)malloc(sizeof(int)*character->moveSteps);
	character->moveY = (int *)malloc(sizeof(int)*character->moveSteps);

	// Set move vectors
	if(dx){
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x + ( (i *  dx ) / character->moveSteps ) ;			
			character->moveY[i] = m*character->moveX[i] + b; 
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x - 16;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y - 32;
	}else{
		for(i=0; i < character->moveSteps-1; i++){
			character->moveX[i] = character->rcDest.x;			
			character->moveY[i] = character->rcDest.y + ( (i *  dy ) / character->moveSteps ) ;	
		}
		character->moveX[character->moveSteps-1] = cursor->rcDest.x - 16;
		character->moveY[character->moveSteps-1] = cursor->rcDest.y - 32;
	}
}

void CharacterMove(Character *character, Map *map)
{
	int coordNwX, coordNwY, coordNeX, coordNeY, coordSwX, coordSwY, coordSeX, coordSeY;
	int firstX, firstY;
	int coordX, coordY;
	
	// Set the coordenates, if character is moving
	if( character->actualStep < character->moveSteps ){
		
		// Get character coordenate
		GetCoor(character->moveX[0]+16+16, character->moveY[0]+32+16, 
				&firstX, &firstY);
		
		// Get north west corner coordenate
		GetCoor(character->moveX[character->actualStep]+16+COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+COLLISIONS_DEPTH, 
				&coordNwX, &coordNwY);
		
		// Get north east corner coordanate
		GetCoor(character->moveX[character->actualStep]+16+32-COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+COLLISIONS_DEPTH, 
				&coordNeX, &coordNeY);
		
		// Get south west corner coordenate 
		GetCoor(character->moveX[character->actualStep]+16+COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+32-COLLISIONS_DEPTH, 
				&coordSwX, &coordSwY);
		
		// Get south east corner coordenate
		GetCoor(character->moveX[character->actualStep]+16+32-COLLISIONS_DEPTH, 
				character->moveY[character->actualStep]+32+32-COLLISIONS_DEPTH, 
				&coordSeX, &coordSeY);

		// Check collision and skip first coordenate for the initial movement
		if(((map->charPosition[coordNwY][coordNwX] || map->collisions[coordNwY][coordNwX]) || 
		    	(map->charPosition[coordNeY][coordNeX] || map->collisions[coordNeY][coordNeX]) ||
		    	(map->charPosition[coordSwY][coordSwX] || map->collisions[coordSwY][coordSwX]) || 
				(map->charPosition[coordSeY][coordSeX] || map->collisions[coordSeY][coordSeX])) && 
		    	(!map->charPosition[firstY][firstX])){
			
			character->actualStep = character->moveSteps;
			
			character->collision = 1;

			goto fin; 
		}else{
			character->rcDest.x = character->moveX[character->actualStep];
			character->rcDest.y = character->moveY[character->actualStep];
			character->actualStep++;
		}
	
		// Animation
		character->rcSrc.y = character->moveOrient * SPRITE_SIZE;
		if(character->skipFrames >= NUM_SKIP_FRAMES) {
			if (character->moveState == 4)
				character->moveState = 1;
			else
				character->moveState++;
			character->skipFrames = 0;
		}else
			character->skipFrames++;
	}else{
fin:
		character->moveState = 0;
		character->moving = 0;
		
		// Set to 0 character position
		GetCoor(character->rcDest.x + 16, character->rcDest.y + 32, &coordX, &coordY);

		map->charPosition[coordY][coordX] = 1;
		//free(character->moveX);
		//free(character->moveY);
	}
	character->rcSrc.x = character->moveState * SPRITE_SIZE;
}

void CharacterDraw(Character* character, SDL_Surface* screen, Map *map)
{
	SDL_Rect rc192;

	if(character->moving)
		SDL_BlitSurface(character->sprite, &character->rcSrc, map->surfaceBack, &character->rcDest);
	else if(character->attacking){
		if(character->attackType == SLASH)
			SDL_BlitSurface(character->spriteSlash, &character->rcSrcAttack, map->surfaceBack, &character->rcDest);
		else if(character->attackType == SPELL)
			SDL_BlitSurface(character->spriteSpell, &character->rcSrcAttack, map->surfaceBack, &character->rcDest);
		else if(character->attackType == SLASH192){
			rc192.x = character->rcDest.x - SPRITE_SIZE;
			rc192.y = character->rcDest.y - SPRITE_SIZE;
			SDL_BlitSurface(character->sprite192Slash, &character->rcSrcAttack, map->surfaceBack, &rc192);		
		}
	}else
		SDL_BlitSurface(character->sprite, &character->rcSrc, map->surfaceBack, &character->rcDest);	
}

void CharacterSetAttack(Character *character, AttackType type)
{
	// Set status attack
	character->attacking  		= 1;
	character->actualAttackStep	= 0;
	character->attackState  	= 1;
	character->skipFrames 		= 0;
	character->attackType 		= type;
	switch(type){
		case SLASH:
			character->attackSteps = 4;
			break;
		case SPELL:
			character->attackSteps = 3;
			break;
		case SLASH192:
			character->attackSteps = 4;
			break;
		default:
			break;
	}
}

void CharacterAttack(Character *character)
{
	int spriteSize = SPRITE_SIZE;

	if((character->attackType == SLASH) || (character->attackType == SPELL)){
		spriteSize = SPRITE_SIZE;
		character->rcSrcAttack.w = SPRITE_SIZE;
		character->rcSrcAttack.h = SPRITE_SIZE;
	}else if(character->attackType == SLASH192){
		spriteSize = 192;
		character->rcSrcAttack.w = 192;
		character->rcSrcAttack.h = 192;
	}

	if((character->actualAttackStep < character->attackSteps) && character->attacking){
		character->rcSrcAttack.y = character->moveOrient * spriteSize;
		if(character->skipFrames == NUM_SKIP_FRAMES) {
			if ((character->attackState == 5) && ((character->attackType == SLASH) || (character->attackType == SLASH192)))
				character->attackState = 1;
			else if ((character->attackState == 4) && (character->attackType == SPELL))
				character->attackState = 1;	
			else{
				character->attackState++;
				character->actualAttackStep++;
			}
			character->skipFrames = 0;
		}else{
			character->skipFrames++;
		}
	}else{
		character->attackState = 0;
		character->attacking = 0;
	}
	character->rcSrcAttack.x = character->attackState * spriteSize;
	
}

void CharacterUpdate(Character *character)
{
    switch(character->state){
        case MOVING:
            break;
        case ATTACKING:
            break;
        default:
            break;
    }
}
