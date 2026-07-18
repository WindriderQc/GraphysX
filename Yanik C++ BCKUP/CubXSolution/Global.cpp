#include "StdAfx.h"
#include "Global.h"


// Initialisation du singleton à NULL
CLGlobalVar *CLGlobalVar::_singleton = NULL;


CLGlobalVar::CLGlobalVar(void)
{
 SetbDEBUG_MODE(false);
 SetbTVSHADOWS(true);
 SetbCAM_CTRL(true);
 SetbMOUSE_CAM_CTRL(false);
 SetbCEGUI_ACTIVE(true);
 SetbLOAD_ATMOSPHERE(true);
 SetbUSE_TV_PHYSICS(true);
 SetbFULLSCREEN(false); 
 SetRender3DAxis(true);
 SetRenderAtmos(true);
 SetShowFPS(false);
 SetScreensaverEnabled(false);
 SetScreensaverRunning(false);
 SetCEGUIWindowsOpened(false);
 SetActiveScene(NULL);
}
