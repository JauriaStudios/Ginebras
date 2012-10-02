#include "textbox.h"

void TextboxCreateWindow(Textbox * textbox, SDL_Surface* screen);

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
	textbox->textMsg = "Ginebras";
	
	//Setup the location on the screen to blit to

	// Text position
	textbox->rcDestText.x = 75;
	textbox->rcDestText.y = 65;
	
	// Window Position
	textbox->windowX = 60;
	textbox->windowY = 50;

	// Window tiles
	textbox->rcSrcBox.x = 0;
	textbox->rcSrcBox.y = 0;
	textbox->rcSrcBox.w = 16;
	textbox->rcSrcBox.h = 16;

	// Window size
	textbox->windowW = 8;
	textbox->windowH = 2;
	

 	
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
	

	//printf("UPDATE");
	
	sprintf(tmpMsg,"X:%d  Y:%d", scrollX, scrollY);
	//printf("%s", tmpMsg);	
	textbox->textMsg = tmpMsg;
}

int TextboxDraw(Textbox * textbox, SDL_Surface* screen)
{
	textbox->message = TTF_RenderText_Solid(textbox->font, textbox->textMsg, textbox->textColor);
	
	if (textbox->message != NULL) {
		TextboxCreateWindow(textbox, screen);
		//SDL_BlitSurface(textbox->background , NULL, screen, &textbox->rcDestText);
		SDL_BlitSurface(textbox->message , NULL, screen, &textbox->rcDestText);
		SDL_FreeSurface(textbox->message);
		return 1;
	}
	else {
		// report error
		return 0;
	}
}

void TextboxCreateWindow(Textbox * textbox, SDL_Surface* screen)
{

	int x = textbox->windowX; 
	int y = textbox->windowY;

	int width = textbox->windowW;
	int height = textbox->windowH;

	int i, j;
		
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
			textbox->rcDestBox.x = x+(16*i);
			textbox->rcDestBox.y = y+(16*j);

			SDL_BlitSurface(textbox->bgTileset, &textbox->rcSrcBox, screen, &textbox->rcDestBox);
		}
	}

}
void TextboxDestructor(Textbox * textbox)
{
	//printf("Destructor");
	
	//Free the text_surface surface
	SDL_FreeSurface(textbox->message);
	SDL_FreeSurface(textbox->background);
	SDL_FreeSurface(textbox->bgTileset);
	
	//Close the font that was used
	TTF_CloseFont(textbox->font);
	TTF_Quit();
	SDL_Quit();
}

