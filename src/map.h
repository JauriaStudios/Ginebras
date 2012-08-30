#ifndef _MAP_H_
#define _MAP_H_

#include "SDL.h"
#include "SDL_image.h"
//#include "cursor.h"

/*****************************************************
 ******* STRUCTS *************************************
 *****************************************************/

typedef struct Map{

	SDL_Surface*	surfaceBackground[40][40];
	SDL_Rect	rcGrassSrc;
	SDL_Rect	rcGrassDest;

	FILE* pInput;

	int background[40][40];
	//int objects[MAP_SIZE_X][MAP_SIZE_Y];
	//int collisions[MAP_SIZE_X][MAP_SIZE_Y];

} Map;


/*****************************************************
 ******* METHODS *************************************
 *****************************************************/

extern Map* MapConstructor();
extern void MapLoad(Map * map, char* file);
extern void MapUpdate(Map * map);
extern void MapDraw(Map * map, SDL_Surface* screen);
extern void MapClean(Map * map);
SDL_Surface* loadImage2lol(char* filename);

#endif
