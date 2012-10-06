#ifndef _GAMES_UTILITIES_H_
#define _GAMES_UTILITIES_H_

#include <math.h>

#include "SDL.h"
#include "SDL_image.h"

#define SPRITE_SIZE 64

#define TILE_SIZE 32

typedef enum Orientation {
	ORIENT_NORTH,
	ORIENT_WEST,
	ORIENT_SOUTH,
	ORIENT_EAST,
}Orientation;

extern void copySurface (SDL_Surface *source, SDL_Rect *src,
                       SDL_Surface *destiny, SDL_Rect *dest);
extern SDL_Surface* loadImage(char *filename);
extern void GetCoor(int pixelX, int pixelY, int *coordX, int *coordY);
extern void overSurface (SDL_Surface *source, SDL_Rect *src, SDL_Surface *destiny, SDL_Rect *dest, int mapWidth);

#endif // _GAMES_UTILITIES_H_
