#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"
#include "map.h"

typedef struct Cursor{

	SDL_Surface* sprite;
	SDL_Rect	 rcSrc;
	SDL_Rect     rcDest; 
	int 		 **collisionArea;
	int 		 coordX, coordY;
	int			 sideLength;
	int 		 free;
} Cursor;

extern Cursor* CursorConstructor(int x0, int y0);
extern void CursorDestructor(Cursor *this);
extern void CursorDraw(Cursor* cursor, SDL_Surface* screen, Map *map);
extern void CursorMove(Cursor* cursor, Orientation orientation);
extern void CursorSetPosition(Cursor *cursor, int x, int y);
extern void CursorSetCollisionArea(Cursor *cursor, int area);

#endif // _CURSOR_H_
