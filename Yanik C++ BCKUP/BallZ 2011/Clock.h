#pragma once
#include "StdAfx.h"

#define MILLISEC_PER_DAY 86400000


class CLClock
{

private:
	CLClock(void);
	~CLClock(void);

public:
	void Init(long StartTime, long TimeScale);
	void Reset(void);
	void update(float elaspedTimeLastFrame);
    long getRealTime(void); 
    long getVirtualTime(void);
    int getDay(void);
    long getTimeOfDay(void);



  // Fonctions de création et destruction du singleton
  static CLClock *getInstance ()
  {
    if (NULL == _singleton)
      {
        _singleton =  new CLClock;
      }

    return _singleton;
  }

  static void kill ()
  {
    if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
  }



private:
		static CLClock *_singleton;

		//time 0 corresponds to 12:00:00 AM on day 0
		float fTotalTimeElapsed;  //real milliseconds since start of simulation
		long lTimeScale;  // virtual milliseconds per real milliseconds
		long lStartTime;
};

