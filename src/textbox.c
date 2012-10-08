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

Textbox *TextboxConstructor(SDL_Surface *screen)
{
	// Variable definition section
	
	Textbox * textbox;
	
	// Alloc map
	textbox = (Textbox *)malloc(sizeof(Textbox));
	
	// initialize sdl ttf
	if (TTF_Init() == -1) {
		printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	
	}
	
	//init fonts
	textbox->font = NULL;
	textbox->fontMono = NULL;

	// Load fonts and images
	TextboxLoad(textbox);

	// define text color
	textbox->textColor.r = 255;
	textbox->textColor.g = 255;
	textbox->textColor.b = 255;
	
	// message to display
	textbox->textMsg = "Atacar";
	
	//Setup the location on the screen to blit to

	// Text position
	textbox->rcDestText.x = 15;
	textbox->rcDestText.y = 20;
	
	// Window Position in px
	textbox->rcDestWindow.x = 0;
	textbox->rcDestWindow.y = 487;
	
	textbox->windowX = 60;
	textbox->windowY = 50;
	
	// Window tiles
	textbox->rcSrcBox.x = 0;
	textbox->rcSrcBox.y = 0;
	textbox->rcSrcBox.w = 16;
	textbox->rcSrcBox.h = 16;
	
	// Window size in 16x tilesize
	textbox->windowW = 49; //min 3
	textbox->windowH = 6; // min 2

	//create window Test
	TextboxCreateWindow(textbox);

	return textbox;
}

void TextboxLoad(Textbox * textbox)
{
	// load Fonts
	textbox->font = loadFont("data/font/ChronoTrigger.ttf",20);
	textbox->fontMono = loadFont("data/font/ChronoTriggerMonospaced.ttf",30);

	// load Images
	textbox->bgTileset = loadImage("data/windowTileset.png");
}

void TextboxUpdate(Textbox * textbox, int scrollX, int scrollY)
{
	char *tmpMsg;
	tmpMsg = (char *)malloc(sizeof(char)*50);
	
	sprintf(tmpMsg,"X:%d  Y:%d", scrollX, scrollY);
	//printf("%s", tmpMsg);	
	textbox->textMsg = tmpMsg;
}

int TextboxDraw(Textbox * textbox, SDL_Surface* screen)
{
	textbox->message = TTF_RenderText_Solid(textbox->fontMono, textbox->textMsg, textbox->textColor);
	
	if (textbox->message != NULL){ 
		SDL_BlitSurface(textbox->message , NULL, textbox->background, &textbox->rcDestText);
		SDL_BlitSurface(textbox->background , NULL, screen, &textbox->rcDestWindow);
		SDL_FreeSurface(textbox->message);
		return 1;
	}
	else {
		// report error
		return 0;
	}
}

void TextboxCreateWindow(Textbox * textbox)
{
	SDL_Surface* temp = NULL;
	
	//int x = textbox->windowX; 
	//int y = textbox->windowY;
	
	int width = textbox->windowW;
	int height = textbox->windowH;
	
	int i, j;
	
	temp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, (width+1)*16, (height+1)*16, 32, 0, 0, 0, 0x000000FF);
	
	textbox->background = SDL_DisplayFormatAlpha(temp);
	
	for (i = 0; i <= width; i++){
		for (j = 0; j <= height; j++){
			if ((i == 0) && (j == 0)){
				textbox->rcSrcBox.x = 0;
				textbox->rcSrcBox.y = 0;
			}
			else if ((i <  width) && (j == 0)){
				textbox->rcSrcBox.x = 16;
				textbox->rcSrcBox.y = 0;
			}
			else if ((i ==  width) && (j == 0)){
				textbox->rcSrcBox.x = 32;
				textbox->rcSrcBox.y = 0;
			}
			else if ((i ==  width) && (j < height)){
				textbox->rcSrcBox.x = 32;
				textbox->rcSrcBox.y = 16;
			}
			else if ((i == 0 ) && (j < height)){
				textbox->rcSrcBox.x = 0;
				textbox->rcSrcBox.y = 16;
			}
			else if ((i == 0 ) && (j == height)){
				textbox->rcSrcBox.x = 0;
				textbox->rcSrcBox.y = 32;
			}
			else if ((i < width ) && (j == height)){
				textbox->rcSrcBox.x = 16;
				textbox->rcSrcBox.y = 32;
			}
			else if ((i == width ) && (j == height)){
				textbox->rcSrcBox.x = 32;
				textbox->rcSrcBox.y = 32;
			}
			else{
				textbox->rcSrcBox.x = 16;
				textbox->rcSrcBox.y = 16;
			}
			textbox->rcDestBox.x = 16*i;
			textbox->rcDestBox.y = 16*j;
			
			// Function from 
			copySurface(textbox->bgTileset, &textbox->rcSrcBox, textbox->background, &textbox->rcDestBox);
			//SDL_BlitSurface(textbox->bgTileset, &textbox->rcSrcBox,textbox->background, &textbox->rcDestBox);
		}
	}
}

void TextboxDestructor(Textbox * textbox)
{
	
	//Free the text_surface surface
	SDL_FreeSurface(textbox->message);
	SDL_FreeSurface(textbox->background);
	SDL_FreeSurface(textbox->bgTileset);

	//Close the font that was used
	TTF_CloseFont(textbox->font);
	TTF_Quit();
	SDL_Quit();
}

