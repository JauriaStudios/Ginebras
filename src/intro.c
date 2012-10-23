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

	if (!(this->intro = loadImage("data/intro.png"))){
		printf("Intro constructor ERROR: couldn't load Intro\n");
		return NULL;
	}

	if (!(this->logo1 = loadImage("data/logo1.png"))){
		printf("Intro constructor ERROR: couldn't load Intro\n");
		return NULL;
	}
	
	if (!(this->logo2 = loadImage("data/logo2.png"))){
		printf("Intro constructor ERROR: couldn't load Intro\n");
		return NULL;
	}
	
	if (!(this->logo3 = loadImage("data/logo3.png"))){
		printf("Intro constructor ERROR: couldn't load Intro\n");
		return NULL;
	}
	
	
	this->rcIntro.x = (SCREEN_WIDTH >> 1) - (this->intro->w >> 1);
	this->rcIntro.y = 0;

	this->rcLogo1.x = (SCREEN_WIDTH >> 1) - (this->logo1->w >> 1);
	this->rcLogo1.y = (SCREEN_HEIGHT >> 1) - (this->logo1->h >> 1);

	this->rcLogo2.x = (SCREEN_WIDTH >> 1) - (this->logo2->w >> 1);
	this->rcLogo2.y = (SCREEN_HEIGHT >> 1) - (this->logo2->h >> 1);

	this->rcLogo3.x = (SCREEN_WIDTH >> 1) - (this->logo3->w >> 1);
	this->rcLogo3.y = (SCREEN_HEIGHT >> 1) - (this->logo3->h >> 1);

	return this;
}

void IntroDestructor(Intro *this)
{
	SDL_FreeSurface(this->intro);
	SDL_FreeSurface(this->logo1);
	SDL_FreeSurface(this->logo2);
	SDL_FreeSurface(this->logo3);
	free(this);
}

void IntroDraw(Intro *this, SDL_Surface *screen)
{

	SDL_Surface *tmpSurface, *blackFade;
	
	tmpSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
	blackFade = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

	SDL_Delay(1000);

	SDL_BlitSurface(this->logo1, NULL, tmpSurface, &this->rcLogo1);
	fadeScreen(tmpSurface, 1000);

	SDL_BlitSurface(this->logo2, NULL, tmpSurface, &this->rcLogo2);
	fadeScreen(tmpSurface, 1000);

	SDL_BlitSurface(this->logo3, NULL, tmpSurface, &this->rcLogo3);
	fadeScreen(tmpSurface, 1000);

	fadeScreen(blackFade, 1200);

	SDL_BlitSurface(this->intro, NULL, tmpSurface, &this->rcIntro);
	fadeScreen(tmpSurface, 2000);

	SDL_FreeSurface(tmpSurface);
	SDL_FreeSurface(blackFade);	

}
