#pragma once

#define MILLISEC_PER_DAY 86400000

namespace GraphysX
{
	class CLClock
	{
	private:
		//time 0 corresponds to 12:00:00 AM on day 0
		static float fTotalTimeElapsed;  //real milliseconds since start of simulation
		static long lTimeScale; //96;  // virtual milliseconds per real milliseconds
		static long lStartTime;

	public:
		GRAPHYSX_API static void Init(long StartTime, long TimeScale);
		GRAPHYSX_API static void update(float elaspedTimeLastFrame);
		GRAPHYSX_API static void Reset(void);
		GRAPHYSX_API static long getRealTime(void);
		GRAPHYSX_API static long getVirtualTime(void);
		GRAPHYSX_API static  int getDay(void);
		GRAPHYSX_API static long getTimeOfDay(void);
	};

}
namespace GraphysX
{
	class FpsLimiter{
	public:
		FpsLimiter();
		void init(float fmaxFPS);
		void setMaxFPS(float fMaxFPS);

		void beginFrame();
		float endFrame();  // return the current FPS
	private:
		void calculateFPS();

		float fFPS;
		float fMaxFPS = 60.0f;
		float fFrameTime;
		unsigned int iStartTicks;

		
	};
}