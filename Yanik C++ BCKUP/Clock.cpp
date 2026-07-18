#include "Clock.h"
#include <math.h>


CLClock::CLClock(void)
{
	fTotalTimeElapsed = 0;
    lTimeScale = 2000; //96
}
CLClock::CLClock(long StartTime, long TimeScale)
{
	fTotalTimeElapsed = StartTime * TimeScale;
    lTimeScale = TimeScale; 
}

CLClock::~CLClock(void)
{
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
