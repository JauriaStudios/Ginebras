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

	char *name;
	
	SDL_Surface *background;
	SDL_Surface *bgTileset;
	SDL_Surface *image;

	SDL_Rect rcDestText, rcDestTile, rcSrcTile, rcDestWindow ;
	
	SDL_Color textColor;
	
	TTF_Font *font;
	TTF_Font *fontMono;
	
	int boxWidth, boxHeight;
	
	int rows;
	
	char **textMsg;

	struct list_head list;

} Textbox;


/**********************************************************
 *** METHODS
 **********************************************************/
extern Textbox* TextboxConstructor(char *name, int x, int y, int w, int h, char **text, int rows, char *image);
extern void TextboxDestructor(Textbox *textbox);
extern void TextboxCreateWindow(Textbox * textbox);
extern void TextboxUpdate(Textbox *textbox, int scrollX, int scrollY);
extern int TextboxDraw(Textbox *textbox, SDL_Surface *screen);


#endif
