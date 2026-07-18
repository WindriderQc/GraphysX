#include "Clock.h"
#include <math.h>

// suppression de warning a la compilation 
#pragma warning(disable: 4244) // conversion double -> float 

// Initialisation du singleton à NULL
CLClock *CLClock::_singleton = NULL;


CLClock::CLClock(void)
{
	fTotalTimeElapsed = 0;
    lTimeScale = 2000; //96
	lStartTime = 0;
}

CLClock::~CLClock(void)
{
}

void CLClock::Init(long StartTime, long TimeScale)
{
	lStartTime = StartTime;
	fTotalTimeElapsed = lStartTime * TimeScale;
    lTimeScale = TimeScale; 
}

void CLClock::Reset(void)
{
	fTotalTimeElapsed = 0 + lStartTime;
}

void CLClock::update(float elaspedTimeLastFrame)
{
  fTotalTimeElapsed += elaspedTimeLastFrame;

}
   
long CLClock::getRealTime(void)
{
  return(fTotalTimeElapsed);
}

long CLClock::getVirtualTime(void)
{
  return(fTotalTimeElapsed * lTimeScale);
}

int CLClock::getDay(void)
{
  return(int(floor(fTotalTimeElapsed * lTimeScale / MILLISEC_PER_DAY)));
}

long CLClock::getTimeOfDay(void)
{
	int Day = getDay();
  return((long(fTotalTimeElapsed) * lTimeScale) - Day * MILLISEC_PER_DAY);
}
