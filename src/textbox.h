#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "list.h"
#include "gameUtilities.h"

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Textbox{	
	
	SDL_Surface *background;
	SDL_Surface *message;
	SDL_Rect     rcText, rcBox;
	
	TTF_Font *font;
	SDL_Color textColor;
	
	char *textMsg;
	
} Textbox;


/**********************************************************
 *** METHODS
 **********************************************************/
extern Textbox* TextboxConstructor(SDL_Surface *screen);
extern void TextboxDestructor(Textbox *textbox);
extern void TextboxLoad(Textbox *textbox);
extern void TextboxUpdate(Textbox *textbox);
extern int TextboxDraw(Textbox *textbox, SDL_Surface *screen);

#endif
