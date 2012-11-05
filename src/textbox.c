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

#include "textbox.h"

TTF_Font* loadFont(char* file, int ptsize)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, ptsize);
	if (tmpfont == NULL){
		printf("Unable to load font: %s %s \n", file, TTF_GetError());
		// Handle the error here.
	}
	return tmpfont;
}

Textbox *TextboxConstructor(char *name ,int x, int y, int w, int h, char **text, int rows, char *image, Menu *menu)
{
	// Variable definition section
	Textbox * textbox;
	SDL_Rect rcSrcImg, rcDestImg;

	// Alloc map
	textbox = (Textbox *)malloc(sizeof(Textbox));
	
	textbox->name = name; 

	// initialize sdl ttf
	if (TTF_Init())
		printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	
	//init fonts
	textbox->font = NULL;
	textbox->fontMono = NULL;

	// load Fonts
	textbox->font = loadFont("data/font/ChronoTrigger.ttf",20);
	textbox->fontMono = loadFont("data/font/ChronoTriggerMonospaced.ttf",18);

	// load Window tileset
	if(!(textbox->bgTileset = loadImage("data/windowTileset.png"))){
		printf("TEXBOX CONSTRUCTOR FAIL: impossible load data/windowTileset.png file\n");
		return NULL;
	}

	// define text color
	textbox->textColor.r = 255;
	textbox->textColor.g = 255;
	textbox->textColor.b = 255;
	// define text color
	textbox->textHigh.r = 255;
	textbox->textHigh.g = 0;
	textbox->textHigh.b = 0;
	
	// Window Position in px
	textbox->rcDestWindow.x = x;
	textbox->rcDestWindow.y = y;

	// Text position
	textbox->rcDestText.x = textbox->rcDestWindow.x + 12;
	textbox->rcDestText.y = textbox->rcDestWindow.y - 10;
		
	// Window tiles
	textbox->rcSrcTile.x = 0;
	textbox->rcSrcTile.y = 0;
	textbox->rcSrcTile.w = 16;
	textbox->rcSrcTile.h = 16;
	
	// Window size in 16x tilesize
	textbox->boxWidth = w; //min 3
	textbox->boxHeight = h; // min 2

	// Create window Test
	TextboxCreateWindow(textbox);

	// Load image
	if(image){
		rcSrcImg.x = 0;
		rcSrcImg.y = 0;
		rcSrcImg.w = 135;
		rcSrcImg.h = 135;
		rcDestImg.x = 10;
		rcDestImg.y = 5;
		if(!(textbox->image = loadImage(image))){
			printf("Text Box constructor ERROR: impossible load %s\n", image);
			return NULL;
		}
		
		overSurface(textbox->image, &rcSrcImg, textbox->background, &rcDestImg, 135);
	}

	// Load menu
	if(menu){
		textbox->menu = menu;
	}else
		textbox->menu = NULL;

	// Set text
	textbox->textMsg = text;
	textbox->rows    = rows;
	INIT_LIST_HEAD(&textbox->list);

	textbox->visible = 1;
	return textbox;
}

void TextboxUpdate(Textbox * textbox, int scrollX, int scrollY)
{
	//char *tmpMsg;
	//tmpMsg = (char *)malloc(sizeof(char)*50);
	
	//sprintf(tmpMsg,"X:%d  Y:%d", scrollX, scrollY);
	//printf("%s", tmpMsg);	
	//textbox->textMsg = tmpMsg;
}

int TextboxDraw(Textbox *textbox, SDL_Surface* screen)
{
	int i;
	SDL_Surface *message = NULL;
	SDL_Rect rcDest;


	// Set menu text
	if(textbox->menu){
		textbox->textMsg = textbox->menu->actualMenu;
		textbox->rows = textbox->menu->actualMenuRows;
		textbox->visible = textbox->menu->visible;
	}

	if(textbox->visible){
		SDL_BlitSurface(textbox->background, NULL, screen, &textbox->rcDestWindow);

		rcDest.x = textbox->rcDestText.x;
		rcDest.y = textbox->rcDestText.y;
		for(i = 0; (i < textbox->rows) && textbox->textMsg; i++){
			if(textbox->menu && (i == textbox->menu->colorRow)){
				message = TTF_RenderText_Solid(textbox->fontMono, textbox->textMsg[i], textbox->textHigh);
			}else
				message = TTF_RenderText_Solid(textbox->fontMono, textbox->textMsg[i], textbox->textColor);

			if(!message){
				printf("TextBoxDraw ERROR: impossible draw %d line, does not exist\n", i);
				return -1;
			}
			rcDest.y += 20;
			SDL_BlitSurface(message, NULL, screen, &rcDest);
			SDL_FreeSurface(message);
		}
	}

	return 0;

}

void TextboxCreateWindow(Textbox * textbox)
{
	SDL_Surface* temp = NULL;
	
	int width = textbox->boxWidth/16;
	int height = textbox->boxHeight/16;
	
	int i, j;
	
	temp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, (width+1)*16, (height+1)*16, 32, 0, 0, 0, 0x000000FF);
	
	textbox->background = SDL_DisplayFormatAlpha(temp);
	
	for (i = 0; i <= width; i++){
		for (j = 0; j <= height; j++){
			if ((i == 0) && (j == 0)){
				textbox->rcSrcTile.x = 0;
				textbox->rcSrcTile.y = 0;
			}
			else if ((i <  width) && (j == 0)){
				textbox->rcSrcTile.x = 16;
				textbox->rcSrcTile.y = 0;
			}
			else if ((i ==  width) && (j == 0)){
				textbox->rcSrcTile.x = 32;
				textbox->rcSrcTile.y = 0;
			}
			else if ((i ==  width) && (j < height)){
				textbox->rcSrcTile.x = 32;
				textbox->rcSrcTile.y = 16;
			}
			else if ((i == 0 ) && (j < height)){
				textbox->rcSrcTile.x = 0;
				textbox->rcSrcTile.y = 16;
			}
			else if ((i == 0 ) && (j == height)){
				textbox->rcSrcTile.x = 0;
				textbox->rcSrcTile.y = 32;
			}
			else if ((i < width ) && (j == height)){
				textbox->rcSrcTile.x = 16;
				textbox->rcSrcTile.y = 32;
			}
			else if ((i == width ) && (j == height)){
				textbox->rcSrcTile.x = 32;
				textbox->rcSrcTile.y = 32;
			}
			else{
				textbox->rcSrcTile.x = 16;
				textbox->rcSrcTile.y = 16;
			}
			textbox->rcDestTile.x = 16*i;
			textbox->rcDestTile.y = 16*j;
			
			// apply tiles to textbox background
			copySurface(textbox->bgTileset, &textbox->rcSrcTile, textbox->background, &textbox->rcDestTile);
		}
	}
}

void TextboxDestructor(Textbox * textbox)
{
	// Free image
	//if(textbox->image)
		//SDL_FreeSurface(textbox->image);

	// Free menu
	//if(textbox->menu)
	//	MenuDestructor(textbox->menu);

	//Free the text_surface surface
	SDL_FreeSurface(textbox->background);
	SDL_FreeSurface(textbox->bgTileset);

	//Close the font that was used
	TTF_CloseFont(textbox->font);
	TTF_Quit();
	SDL_Quit();
}

