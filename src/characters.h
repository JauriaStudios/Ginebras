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
} AttackType;

typedef struct Character{

	SDL_Surface* sprite;
	SDL_Surface* spriteSlash;
	SDL_Surface* sprite192Slash;
	SDL_Rect	 rcSrc, rcSrcAttack;
	SDL_Rect     rcDest;
	
	DestinationPoint destinationPoint;
	int velocity;

	struct list_head list;
	struct list_head listSort;

	// move
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
	int attackType;

	// Collisions
	int collisionX;
	int collisionY;
	int orientCol;
	int collision;

} Character;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Character* CharacterConstructor(char* file, Orientation or, int x0, int y0, int iniciative, int movement, int **pos);
extern void CharacterDestructor(Character *character);
extern void CharacterSetDestination(Character* character, Cursor* cursor, Map *map);
extern void CharacterMove(Character *character, Map *map);
extern void CharacterDraw(Character* character, SDL_Surface* screen, Map *map);
extern void CharacterSetAttack(Character *character, int type);
extern void CharacterAttack(Character* character);

#endif //_CHARACTER_H_
