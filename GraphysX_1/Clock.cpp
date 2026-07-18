#include "StdAfx.h"
#include "Clock.h"
#include <math.h>

//TODO: combine FPSLimiter in the class object

namespace GraphysX
{
	//time 0 corresponds to 12:00:00 AM on day 0
	float CLClock::fTotalTimeElapsed = 0;  //real milliseconds since start of simulation
	long CLClock::lTimeScale = 2000; //96;  // virtual milliseconds per real milliseconds
	long CLClock::lStartTime = 0;


	void CLClock::Init(long StartTime, long TimeScale)
	{
		lStartTime = StartTime;
		fTotalTimeElapsed = float(lStartTime * TimeScale);
		lTimeScale = TimeScale;
		std::cout << "Clock created: Time 00:00:00" << std::endl;  // TODO: setter la string pour que le display soit avec le start time et timescale
	}
	void CLClock::Reset(void)
	{
		fTotalTimeElapsed = float(0 + lStartTime);
	}
	void CLClock::update(float elaspedTimeLastFrame)
	{
		fTotalTimeElapsed += elaspedTimeLastFrame;
	}
	long CLClock::getRealTime(void)
	{
		return((long)fTotalTimeElapsed);
	}
	long CLClock::getVirtualTime(void)
	{
		return((long)fTotalTimeElapsed * lTimeScale);
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
}
#include <SDL\SDL.h>
namespace GraphysX
{
	FpsLimiter::FpsLimiter(){}
	void FpsLimiter::init(float fmaxFPS) {
		setMaxFPS(fmaxFPS);
	}
	void FpsLimiter::setMaxFPS(float fmaxFPS)
	{
		fMaxFPS = fmaxFPS;
	}

	void FpsLimiter::beginFrame() {
		iStartTicks = SDL_GetTicks();
	}
	float FpsLimiter::endFrame() {
		// Limits the FPS to fMaxFPS 
		calculateFPS();    //  devrait inclure le fps limiter pour éviter de double les SDLGetTick
		
		unsigned int frameTicks = SDL_GetTicks() - iStartTicks;
		if (1000.0f / fMaxFPS > frameTicks)
		{
			SDL_Delay(Uint32(1000.0f / fMaxFPS - frameTicks));//Limit the FPS
		}
		return fFPS; 
		// return the current FPS
	}  
	void FpsLimiter::calculateFPS(){
			static const int NUM_SAMPLES = 100;  // number of frame sample per FPS count
			static float frameTimes[NUM_SAMPLES];
			static int currentFrame = 0;

			static float prevTicks = (float)SDL_GetTicks();
			float currentTicks;
			currentTicks = (float)SDL_GetTicks();
			fFrameTime = currentTicks - prevTicks;
			frameTimes[currentFrame % NUM_SAMPLES] = fFrameTime;

			prevTicks = currentTicks;
			currentFrame++;

			int count;
			if (currentFrame < NUM_SAMPLES)  {
				count = currentFrame;
			}
			else {
				count = NUM_SAMPLES;
			}

			float frameTimeAverage = 0;
			for (int i = 0; i < count; i++)  {
				frameTimeAverage += frameTimes[i];
			}
			frameTimeAverage /= count;


			if (frameTimeAverage > 0) {
				fFPS = 1000.0f / frameTimeAverage;
			}
			else {
				fFPS = 0.0f;
			}

	}

}