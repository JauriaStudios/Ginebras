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
