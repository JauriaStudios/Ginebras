#include <stdio.h>

#include "timer.h"
#include "SDL.h"

Timer* TimerConstructor(void)
{
	Timer* timer;
	timer = (Timer *)malloc(sizeof(Timer));
	timer->startTicks  = 0;
	timer->pausedTicks = 0;
	timer->paused	   = 0;
	timer->started	   = 0;

	return timer;
}

void TimerStart(Timer *timer)
{
	// Start the timer
	timer->started = 1;

	// Unpaused the timer
	timer->paused = 0;

	timer->startTicks = SDL_GetTicks();
}

void TimerStop(Timer *timer)
{
	timer->started = 0;
	timer->paused  = 0;
}

void TimerPause(Timer *timer)
{
    //If the timer is running and isn't already paused
    if( ( timer->started ) && ( !timer->paused ) ){
        //Pause the timer
        timer->paused = 1;
 
        //Calculate the paused ticks
        timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
    }
}

