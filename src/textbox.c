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
	//printf("Constructor:\n");
	// Variable definition section
	
	Textbox * textbox;
	
	// Alloc map
	textbox = (Textbox *)malloc(sizeof(Textbox));
	
	// initialize sdl ttf
	if (TTF_Init() == -1) {
		printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	
	}
	
	textbox->font = NULL;
	textbox->fontMono = NULL;

	TextboxLoad(textbox);

	textbox->textColor.r = 255;
	textbox->textColor.g = 255;
	textbox->textColor.b = 255;
	
	textbox->textMsg = "Ginebras";
	
	//textbox->font = loadfont("data/font/STIX.otf", 16); 	
	//Setup the location on the screen to blit to
	textbox->rcText.x = 40;
	textbox->rcText.y = 30;

	textbox->rcBox.x = 60;
	textbox->rcBox.y = 50;
 	
	return textbox;
}

void TextboxLoad(Textbox * textbox)
{
			//printf("LOAD");
	// load Fonts
	textbox->font = loadFont("data/font/ChronoTrigger.ttf",20);
	textbox->fontMono = loadFont("data/font/ChronoTriggerMonospaced.ttf",30);
	// load Images

	textbox->background = loadImage("data/window.png");
	
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
		SDL_BlitSurface(textbox->background , NULL, screen, &textbox->rcText);
		SDL_BlitSurface(textbox->message , NULL, screen, &textbox->rcBox);
		SDL_FreeSurface(textbox->message);
		return 1;
	}
	else {
		// report error
		return 0;
	}
}

void TextboxDestructor(Textbox * textbox)
{
	//printf("Destructor");
	
	//Free the text_surface surface
	SDL_FreeSurface(textbox->message);
	
	//Close the font that was used
	TTF_CloseFont(textbox->font);
	TTF_Quit();
	SDL_Quit();
}

