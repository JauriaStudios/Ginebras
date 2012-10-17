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
