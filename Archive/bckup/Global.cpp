#include "stdafx.h"
#include "Global.h"

// Initialisation du singleton à NULL
CLGlobalVar *CLGlobalVar::_singleton = NULL;

CLGlobalVar::CLGlobalVar()
{
	fBallSpeed = 0;
	fDayNightAlpha = 0;
	bRender3DAxis = true;
	bFULLSCREEN = false;
	bDEBUG_MODE = true;
	bMOUSE_CAM_CTRL = true;
	w3DWindowHandle = NULL;
	SearchPath = "";
	fTimeElapsed = 0;
}
CLGlobalVar::~CLGlobalVar() 
{ }
