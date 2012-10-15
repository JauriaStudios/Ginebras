#include "intro.h"

Intro* IntroConstructor()
{
	Intro *this;
	this = (Intro *)malloc(sizeof(Intro));

	this->intro = loadImage("data/intro.png");
	
	this->rcIntro.x = (SCREEN_WIDTH >> 1) - (this->intro->w >> 1);
	this->rcIntro.y = 0;

	return this;
}

void IntroDestructor(Intro *this)
{
	SDL_FreeSurface(this->intro);
	free(this);
}

void IntroDraw(Intro *this, SDL_Surface *screen)
{
	SDL_Surface *blackFade, *tmpSurface;
	
	int w = this->intro->w;
	int h = this->intro->h;
	
	blackFade = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
	tmpSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);

	//Wait for one second to create the window
	SDL_Delay(1000);

	int i;
	for(i = 255; i > 0; i--){

		SDL_SetAlpha(blackFade, SDL_RLEACCEL | SDL_SRCALPHA, i);

		SDL_BlitSurface(this->intro, NULL, tmpSurface, NULL);
		SDL_BlitSurface(blackFade, NULL, tmpSurface, NULL);

		SDL_BlitSurface(tmpSurface, NULL, screen, &this->rcIntro);

		SDL_Flip(screen);
	}
	SDL_FreeSurface(blackFade);
	SDL_FreeSurface(tmpSurface);

	SDL_BlitSurface(this->intro, NULL, screen, &this->rcIntro);
	SDL_Flip(screen);
}
