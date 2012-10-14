#include "intro.h"

Intro* IntroConstructor()
{
	Intro *this;
	this = (Intro *)malloc(sizeof(Intro));
	this->intro = loadImage("data/intro.png");
	this->rcIntro.x = (SCREEN_WIDTH >> 1) - (this->intro->w >> 1);

	return this;
}

void IntroDestructor(Intro *this)
{
	SDL_FreeSurface(this->intro);
	free(this);
}

void IntroDraw(Intro *this, SDL_Surface *screen)
{
	SDL_BlitSurface(this->intro, NULL, screen, &this->rcIntro);
	SDL_Flip(screen);
}
