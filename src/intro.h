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

#ifndef _INTRO_H_
#define _INTRo_H_

#include "SDL.h"
#include "SDL_image.h"
#include "gameUtilities.h"

/**********************************************************
 *** STRUCTS
 **********************************************************/
typedef struct Intro{
	SDL_Surface  *intro;
	//SDL_Surface *fade2;
	SDL_Rect rcIntro;
	//SDL_Rect rcFade2;
} Intro;

/**********************************************************
 *** METHODS
 **********************************************************/
extern Intro* IntroConstructor();
extern void IntroDraw(Intro *this, SDL_Surface *screen);
extern void IntroDestructor(Intro *this);

#endif // _INTRO_H_
