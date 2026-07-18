#pragma once

#define MILLISEC_PER_DAY 86400000


class CLClock
{

public:
	CLClock(void);
	CLClock(long StartTime, long TimeScale);
	~CLClock(void);


	void update(float elaspedTimeLastFrame);
    long getRealTime(void); 
    long getVirtualTime(void);
    int getDay(void);
    long getTimeOfDay(void);

private:

		//time 0 corresponds to 12:00:00 AM on day 0
		float fTotalTimeElapsed;  //real milliseconds since start of simulation
		long lTimeScale;  // virtual milliseconds per real milliseconds
};

