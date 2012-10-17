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

