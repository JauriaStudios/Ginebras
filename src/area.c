#include "area.h"

Area* AreaConstructor(int x0, int y0, int radius)
{
	// Variable definition section
	Area* this;

	// Alloc memory
	if(!(this = (Area *)malloc(sizeof(Area)))){
		printf("ERROR area: couldn't alloc memory\n");
		return NULL;
	}
	
	// Set radius
	this->radius = radius;
	
	// Load tile sprite for smart draw
	if (!(this->tile = loadImage("data/Peana.png"))){
		printf("ERROR area: couldn't load tile\n");
		return NULL;		
	}

	// Set sprite initial position
	this->rcDest.x = x0 - ((radius-1)*32+16);
	this->rcDest.y = y0 - ((radius-1)*32);
	
	return this;
}

void AreaDestructor(Area *this)
{
	// Free character image
	SDL_FreeSurface(this->tile);

	// Free character
	free(this);
}

void AreaSetPosition(Area *this, int x, int y)
{
	this->rcDest.x = x - ((this->radius-1)*32+16);
	this->rcDest.y = y - ((this->radius-1)*32);
}

void AreaDraw(Area *this, Map *map, Cursor *cursor)
{
	int i, j;
	int x, y;
	SDL_Rect rcDest;	

	// Get area position 
	GetCoor(this->rcDest.x + 16, this->rcDest.y , &x, &y);
	
	//printf("x: %d, y: %d, sideL: %d\n", x, y, cursor->sideLength);
		
	for(i = 0; i < cursor->sideLength; i++){
		for(j = 0; j < cursor->sideLength; j++){
			if((x+j) >= 0 && (y+i) >= 0 && (x+j) < map->width && (y+i) < map->height &&
			   cursor->collisionArea[i][j] && !map->collisions[y+i][x+j] && !map->charPosition[y+i][x+j]){
				rcDest.x = (x+j) * map->tileWidth;
				rcDest.y = (y+i) * map->tileHeight;
				SDL_BlitSurface(this->tile, NULL, map->surfaceBack, &rcDest);	
			}
		}
	}
}
