/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

//#include <toolbox_stdafx.h>
//#include "OpenGlUtil.h"
#include "stdafx.h"
#include "dHighResolutionTimer.h"

#define LOCKED_FPS		float (100.0f)
#define LOCKED_TIMESTEP int ((1000000.0f/LOCKED_FPS))

const dFloat TICKS2SEC = 1.0e-6f;


static unsigned64 m_prevTime = 0;
static LARGE_INTEGER frequency;
static LARGE_INTEGER baseCount;



void dResetTimer()
{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter (&baseCount);
}


unsigned64 dGetTimeInMicroseconds()
{
	
		LARGE_INTEGER count;
		QueryPerformanceCounter (&count);
		count.QuadPart -= baseCount.QuadPart;
		unsigned64 ticks = unsigned64 (count.QuadPart * LONGLONG (1000000) / frequency.QuadPart);
		return ticks;
}


dFloat dGetElapsedSeconds()
{
	dFloat timeStep;
	unsigned64 miliseconds;

	miliseconds = dGetTimeInMicroseconds();

	// optimal keep the fps below 120 fps
	timeStep = dFloat (miliseconds - m_prevTime) * TICKS2SEC;
	m_prevTime = miliseconds;

	return timeStep;
} 

