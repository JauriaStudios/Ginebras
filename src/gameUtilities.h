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

#ifndef _GAMES_UTILITIES_H_
#define _GAMES_UTILITIES_H_

#include <math.h>

#include "SDL.h"
#include "SDL_image.h"

#define SPRITE_SIZE 64

#define TILE_SIZE 32
#define SCREEN_WIDTH  	1280
#define SCREEN_HEIGHT 	800


typedef enum Orientation {
	ORIENT_NORTH,
	ORIENT_WEST,
	ORIENT_SOUTH,
	ORIENT_EAST,
}Orientation;


/* Las funciones que van a utilizarse */
extern void fadeScreen(SDL_Surface *, Uint32);
extern Uint32 getpixel(SDL_Surface *, int, int);
extern void putpixel(SDL_Surface *, int, int, Uint32);

extern void copySurface (SDL_Surface *source, SDL_Rect *src,
                       SDL_Surface *destiny, SDL_Rect *dest);
extern SDL_Surface* loadImage(char *filename);

extern void GetCoor(int pixelX, int pixelY, int *coordX, int *coordY);
extern void overSurface (SDL_Surface *source, SDL_Rect *src, SDL_Surface *destiny, SDL_Rect *dest, int mapWidth);

#endif // _GAMES_UTILITIES_H_
