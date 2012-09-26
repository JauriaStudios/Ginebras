#include "textbox.h"

TTF_Font* loadfont(char* file, int ptsize)
{
	TTF_Font* tmpfont;
	tmpfont = TTF_OpenFont(file, ptsize);
	if (tmpfont == NULL){
		printf("Unable to load font: %s %s \n", file, TTF_GetError());
		// Handle the error here.
	}
	return tmpfont;
}

int DrawText(SDL_Surface* screen, TTF_Font* font, const char* text)
{
    SDL_Color color = {0,0,0};
    SDL_Surface *text_surface;

    text_surface = TTF_RenderText_Solid(font, text, color);
    if (text_surface != NULL)
    {	
        SDL_BlitSurface(text_surface, NULL, screen, NULL);
        SDL_FreeSurface(text_surface);
        return 1;
    }
    else
    {
        // report error
        return 0;
    }
} 

Textbox *TextboxConstructor(SDL_Surface *screen)
{
	printf("Constructor:\n");
	// Variable definition section

	Textbox * textbox;

	// Alloc map
	textbox = (Textbox *)malloc(sizeof(Textbox));
	
	// initialize sdl ttf
	if (TTF_Init() == -1) {
		printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
	
	}
	
	textbox->font = NULL;

	textbox->textColor.r = 255;
	textbox->textColor.g = 255;
	textbox->textColor.b = 255;



	//textbox->font = loadfont("data/font/STIX.otf", 16);
	textbox->font = loadfont("data/font/STIX.otf", 40);

 	
	//Setup the location on the screen to blit to
	textbox->rcDest.x = 5;
	textbox->rcDest.y = 5;
	textbox->rcDest.w = 100;
	textbox->rcDest.h= 50;
 
	return textbox;
}

void TextboxLoad(Textbox * textbox)
{
	printf("LOAD");
}

void TextboxUpdate(Textbox * textbox)
{
	printf("UPDATE");
}


void TextboxDraw(Textbox * textbox, SDL_Surface* screen)
{
	//printf("DRAW");
	//Blit text_surface surface to the screen surface
	DrawText(screen, textbox->font, "LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOL");
}
void TextboxDestructor(Textbox * textbox)
{
	printf("Destructor");
	// close sdl ttf

	//Free the text_surface surface
	SDL_FreeSurface(textbox->message);



	//Free the surfaces SDL_FreeSurface( background );
//	SDL_FreeSurface( message );
	//Close the font that was used TTF_CloseFont( font );
	//Quit SDL_ttf TTF_Quit();
	//Quit SDL SDL_Quit();
}

