#ifndef _GAMES_UTILITIES_H_
#define _GAMES_UTILITIES_H_

#include "SDL.h"
#include "SDL_image.h"

#define SPRITE_SIZE   	64

typedef enum Orientation {
	ORIENT_NORTH,
	ORIENT_WEST,
	ORIENT_SOUTH,
	ORIENT_EAST,
}Orientation;

SDL_Surface *loadImage(char *filename);

#endif // _GAMES_UTILITIES_H_
