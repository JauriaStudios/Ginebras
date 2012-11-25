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

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "SDL.h"
#include "SDL_image.h"
#include "cursor.h"
#include "gameUtilities.h"
#include "area.h"
#include "map.h"
#include "projectile.h"

#define NUM_SKIP_FRAMES   5
#define COLLISIONS_DEPTH  1
#define NUM_ACTIVE_SPELLS 2
#define NUM_PASIVE_SPELLS 1

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

	// Attributes
	int lifeTotal;
	int manaTotal;
	int vigorTotal;

	// Attack
	int attackMelee;
	int attackRange;
	int attackMagic;

	// Defense
	int defensePhysic;
	int defenseMagic;

	// Other
	int critic;
	int evasion;
	int move;
} CharacterAttributes;

typedef struct CharacterSpells {
	void *dataActiveSpells[NUM_ACTIVE_SPELLS];
	void *dataPasiveSpells[NUM_PASIVE_SPELLS];
	int (*activeSpells[NUM_ACTIVE_SPELLS])(void *data);
	int (*pasiveSpells[NUM_PASIVE_SPELLS])(void *data);
} CharacterSpells;

typedef struct CharacterInput {
	Orientation orientation;
	int x0;
	int y0;
	int iniciative;
	int player;
} CharacterInput;

typedef struct Character{

	SDL_Surface* sprite;
	SDL_Surface* spriteSlash;
	SDL_Surface* sprite192Slash;
	SDL_Surface* spriteSpell; 
	SDL_Surface* avatar; 
	SDL_Rect	 rcSrc, rcSrcAttack;
	SDL_Rect     rcDest, rcInitTurn;
	
	CharacterAttributes attributes;
	CharacterSpells		spells;

	char name[50];

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

	// Projectile
	Projectile *projectile;

	// Text for the interface
	char **text;

	// State
	char status[50];

} Character;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Character* CharacterConstructor(xmlNodePtr cur, CharacterInput input, Map *map);
extern void CharacterDestructor(Character *character);
extern void CharacterSetDestination(Character* character, Cursor* cursor, Map *map);
extern void CharacterMove(Character *character, Map *map);
extern void CharacterDraw(Character* character, SDL_Surface* screen, Map *map);
extern void CharacterSetAttack(Character *character, AttackType type);
extern void CharacterAttack(Character* character);
extern void CharacterGetCoor(Character *character, int *x, int *y);
extern int  CharacterCheckEnemy(Character *character, Map *map, int radius, int *xFind, int *yFind);
extern CharacterInput* CharacterFillConfigure(Orientation orientation, int x0, int y0, int iniciative, int player);

#endif //_CHARACTER_H_
