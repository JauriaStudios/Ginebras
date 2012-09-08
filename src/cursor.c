#include "SDL.h"
#include "SDL_image.h"
#include "cursor.h"

Cursor* CursorConstructor(int x0, int y0)
{
	Cursor* cursor;
	cursor = (Cursor *)malloc(sizeof(Cursor));
	// Load sprites image
	if ((cursor->sprite = loadImage("data/cursor.png")) == NULL)
		return NULL;
	
	// Set sprite initial position
	cursor->rcDest.x = x0;
	cursor->rcDest.y = y0;	

	return cursor;
}

void CursorDestructor(Cursor *this)
{
	// Free character image
	SDL_FreeSurface(this->sprite);

	// Free character
	free(this);
}

void CursorDraw(Cursor* cursor, SDL_Surface* screen)
{
	SDL_BlitSurface(cursor->sprite, NULL, screen, &cursor->rcDest);
}

void CursorMove(Cursor* cursor, Orientation orientation)
{
	switch (orientation) {
		case ORIENT_WEST:
			cursor->rcDest.x = cursor->rcDest.x - SPRITE_SIZE; 
			break;
		case ORIENT_EAST:
			cursor->rcDest.x = cursor->rcDest.x + SPRITE_SIZE;
			break;
		case ORIENT_SOUTH:
			cursor->rcDest.y = cursor->rcDest.y + SPRITE_SIZE;
			break;
		case ORIENT_NORTH:
			cursor->rcDest.y = cursor->rcDest.y - SPRITE_SIZE;
			break;
		default:
			break;
	}

}
