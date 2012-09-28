#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "list.h"
#include "gameUtilities.h"

//#include "cursor.h"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH  	800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 	600
#endif

#define BUFFER_SIZE 1024

#define MAP_SIZE_X 40
#define MAP_SIZE_Y 40

#ifndef TILE_SIZE
#define TILE_SIZE 32

#endif

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Textbox{	
	
	SDL_Surface *background;
	SDL_Surface *message;
	SDL_Rect     rcDest;

	TTF_Font* font;
	SDL_Color textColor;
	// message properties
	
	// box properties
	int  x, y, boxWidth, boxHeight;
	
} Textbox;


/**********************************************************
 *** METHODS
 **********************************************************/
extern Textbox* TextboxConstructor(SDL_Surface *screen);
extern void TextboxDestructor (Textbox * textbox);
extern void TextboxLoad (Textbox * textbox);
extern void TextboxUpdate (Textbox * textbox);
extern void TextboxDraw (Textbox * textbox, SDL_Surface* screen);

#endif
