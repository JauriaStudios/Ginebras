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

#ifndef _TIMER_H_
#define _TIMER_H_

typedef struct Timer{
	
	// The clock time when the timer started
	int startTicks;

	//The ticks stored when the timer was paused
    int pausedTicks;

	//The timer status
    unsigned paused;
    unsigned started;
} Timer;

Timer* TimerConstructor(void);
void TimerStart(Timer *timer);
void TimerStop(Timer *timer);
void TimerPause(Timer *timer);

#endif // _TIMER_H_
