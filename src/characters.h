#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <math.h>
#include "list.h"

#include "SDL.h"
#include "SDL_image.h"
#include "cursor.h"
#include "gameUtilities.h"

#define NUM_SKIP_FRAMES 5

/**********************************************************
 **** STRUCTS 
 **********************************************************/
typedef struct DestinationPoint{
	int x;
	int y;
} DestinationPoint;

typedef enum Player {
	PLAYER1,
	PLAYER2,
	IA,
} Player;

typedef struct Character{

	SDL_Surface* sprite;
	SDL_Rect	 rcSrc;
	SDL_Rect     rcDest;
	DestinationPoint destinationPoint;
	int velocity;

	int player;
	struct list_head list;
	struct list_head listSort;

	// move
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

} Character;

/**********************************************************
 **** METHODS 
 **********************************************************/
extern Character* CharacterConstructor(char* file, Orientation or, int x0, int y0, int iniciative);
extern void CharacterDestructor(Character *character);
extern void CharacterSetDestination(Character* character, Cursor* cursor);
extern void CharacterMove(Character *character);
extern void CharacterDraw(Character* character, SDL_Surface* screen);

#endif //_CHARACTER_H_
