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
	SDL_Surface *bgTileset;
	SDL_Surface *message;
	
	SDL_Rect rcDestText, rcDestTile, rcSrcTile, rcDestWindow ;
	
	SDL_Color textColor;
	
	TTF_Font *font;
	TTF_Font *fontMono;
	
	int boxWidth, boxHeight;
	
	
	char *textMsg;
	
} Textbox;


/**********************************************************
 *** METHODS
 **********************************************************/
extern Textbox* TextboxConstructor(SDL_Surface *screen);
extern void TextboxDestructor(Textbox *textbox);
extern void TextboxLoad(Textbox *textbox);
extern void TextboxCreateWindow(Textbox * textbox);
extern void TextboxUpdate(Textbox *textbox, int scrollX, int scrollY);
extern int TextboxDraw(Textbox *textbox, SDL_Surface *screen);


#endif
