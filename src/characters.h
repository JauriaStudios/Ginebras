#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "SDL.h"
#include "SDL_image.h"
//#include "cursor.h"

#define SPRITE_SIZE   	64

/*****************************************************
 ******* STRUCTS *************************************
 *****************************************************/
typedef enum Orientation {
	ORIENT_NORTH,
	ORIENT_WEST,
	ORIENT_SOUTH,
	ORIENT_EAST,
}Orientation;

typedef struct DestinationPoint{
	int x;
	int y;
} DestinationPoint;

typedef struct Character{

	SDL_Surface* sprite;
	SDL_Rect	 rcSrc;
	SDL_Rect     rcDest;
	DestinationPoint destinationPoint;
	int velocity;

	// move
	unsigned moving;
	int moveSteps;
	int actualStep;
	int* moveY;
	int* moveX;
	Orientation moveOrient;
	int moveState; // 1, 2 ... 8 ; 0 stop   

} Character;

typedef struct Cursor{

	SDL_Surface* sprite;
	SDL_Rect	 rcSrc;
	SDL_Rect     rcDest; 

} Cursor;

/*****************************************************
 ******* METHODS *************************************
 *****************************************************/
extern Character* CharacterConstructor(char* file, Orientation or, int x0, int y0);
extern void CharacterSetDestination(Character* character, Cursor* cursor);
extern void CharacterMove(Character *character);
extern void CharacterDraw(Character* character, SDL_Surface* screen);

extern Cursor* CursorConstructor(int x0, int y0);
extern void CursorDraw(Cursor* cursor, SDL_Surface* screen);
extern void CursorMove(Cursor* cursor, Orientation orientation);

SDL_Surface *loadImage(char *filename);

#endif

