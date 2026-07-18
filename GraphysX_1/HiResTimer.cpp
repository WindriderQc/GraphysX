#include "stdafx.h"
#include "HiResTimer.h"

namespace Tools
{
	static unsigned64 m_prevTime = 0;
	static LARGE_INTEGER frequency;
	static LARGE_INTEGER baseCount;

	void resetTimer()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&baseCount);
	}
	unsigned64 getTimeInMicroseconds()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		count.QuadPart -= baseCount.QuadPart;
		unsigned64 ticks = unsigned64(count.QuadPart * unsigned64(1000000) / frequency.QuadPart);
		return ticks;
	}
}