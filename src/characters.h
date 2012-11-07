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

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "list.h"

#include "SDL.h"
#include "SDL_image.h"
#include "cursor.h"
#include "gameUtilities.h"
#include "area.h"
#include "map.h"

#define NUM_SKIP_FRAMES  5
#define COLLISIONS_DEPTH 1

/**********************************************************
 **** STRUCTS 
 **********************************************************/
typedef struct DestinationPoint{
	int x;
	int y;
} DestinationPoint;

typedef enum AttackType {
	SLASH,
	SLASH192,
	SPELL,
} AttackType;

typedef enum CharacterState {
	STOP,
    MOVING,
    ATTACKING,
} CharacterState;

typedef struct CharacterAttributes {
	// Stats
	int strength;
	int agility;
	int vitality;
	int intelligence;
	int spirit;

	// Attributes
	int life;
	int mana;
	int vigor;

	// Attack
	int attackMelee;
	int attackRange;
	int attackMagic;

	// Defense
	int defenseMelee;
	int defenseMagic;

	// Other
	int critic;
	int evasion;
	int move;
} CharacterAttributes;

typedef struct Character{

	SDL_Surface* sprite;
	SDL_Surface* spriteSlash;
	SDL_Surface* sprite192Slash;
	SDL_Surface* spriteSpell; 
	SDL_Rect	 rcSrc, rcSrcAttack;
	SDL_Rect     rcDest, rcInitTurn;
	
	CharacterAttributes attributes;

	DestinationPoint destinationPoint;
	int velocity;

	struct list_head list;
	struct list_head listSort;

	int player;

    // State
    CharacterState state;
	
    // Move
	Area *moveArea;
	int movement;
	unsigned moving;
	int moveSteps;
	int actualStep;
	int* moveY;
	int* moveX;
	int moveOrient;
	int moveState; // 1, 2 ... 8 ; 0 stop   
	// Anti speed
	int skipFrames;

	// Turn
	int iniciative;
	int check;

	// Attack
	int attackSteps;
	int actualAttackStep;
	int attacking;
	int attackState; // 1, 2 ... 5 ; 0 stop
	int attackRadius;
	AttackType attackType;

	// Collisions
	int collisionX;
	int collisionY;
	int orientCol;
	int collision;

} Character;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Character* CharacterConstructor(char* file, Orientation or, int x0, int y0, int iniciative, int movement, int **pos, int areaRadius, int player);
extern void CharacterDestructor(Character *character);
extern void CharacterSetDestination(Character* character, Cursor* cursor, Map *map);
extern void CharacterMove(Character *character, Map *map);
extern void CharacterDraw(Character* character, SDL_Surface* screen, Map *map);
extern void CharacterSetAttack(Character *character, AttackType type);
extern void CharacterAttack(Character* character);
extern void CharacterGetCoor(Character *character, int *x, int *y);
extern int  CharacterCheckEnemy(Character *character, Map *map, int radius, int *xFind, int *yFind);

#endif //_CHARACTER_H_
