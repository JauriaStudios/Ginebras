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
