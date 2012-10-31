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

#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "list.h"
#include "gameUtilities.h"
#include "menu.h"

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

	Menu *menu;	

	int boxWidth, boxHeight;
	
	int rows;
	
	char **textMsg;

	struct list_head list;

} Textbox;


/**********************************************************
 *** METHODS
 **********************************************************/
extern Textbox* TextboxConstructor(char *name, int x, int y, int w, int h, char **text, int rows, char *image, Menu *menu);
extern void TextboxDestructor(Textbox *textbox);
extern void TextboxCreateWindow(Textbox * textbox);
extern void TextboxUpdate(Textbox *textbox, int scrollX, int scrollY);
extern int TextboxDraw(Textbox *textbox, SDL_Surface *screen);


#endif
