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

#include "grid.h"

extern Grid* GridConstructor(int tileSizeX, int tileSizeY)
{
	// Variable definition section
	Grid *this;
	SDL_Surface *tmp;
	SDL_Rect rcDest, rcSrc;
	int x, y;
	
	// Alloc memory
	this = (Grid *)malloc(sizeof(Grid));

	// Load tile
	if(!(this->tile = loadImage("data/Grid.png"))){
		printf("Gird Constructor ERROR: impossible load data/Grid.png\n");
		return NULL;
	}

	// Create image
	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

	this->image = SDL_DisplayFormatAlpha(tmp);
	
	SDL_FreeSurface(tmp);
	
	// Initialize src rectangle
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = tileSizeX;
	rcSrc.h = tileSizeY;

	// Create grid mesh
	for (x = 0; x < SCREEN_WIDTH/tileSizeX; x++) {
		for (y = 0; y < SCREEN_HEIGHT/tileSizeY; y++) {
			rcDest.x = x * tileSizeX;
			rcDest.y = y * tileSizeY;
			printf("%d, %d ", x, y);
			copySurface(this->tile, &rcSrc, this->image, &rcDest);
		}
		printf("\n");
	}
	
	return this;
}

extern void GridDraw(Grid *this, Map *map)
{
	// Variable definition section
	SDL_Rect rcDest;
	
	rcDest.x = -map->scroll_x;
	rcDest.y = -map->scroll_y;
	
	// Draw grid
	SDL_BlitSurface(this->image, NULL, map->surfaceBack, &rcDest);

}
extern void GridDestructor(Grid *this)
{
	// Free grid surfaces
	SDL_FreeSurface(this->tile);
	SDL_FreeSurface(this->image);

	// Free Surface
	free(this);
}

