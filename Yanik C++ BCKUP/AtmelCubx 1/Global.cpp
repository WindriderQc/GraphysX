#include "Global.h"


// Initialisation du singleton à NULL
CLGlobalVar *CLGlobalVar::_singleton = NULL;


void CLGlobalVar::ConfigBooleans(void)
{ 
 SetbDEBUG_MODE(false);
 SetbTVSHADOWS(true);
 SetbCAM_CTRL(true);
 SetbMOUSE_CAM_CTRL(false);
 SetbCEGUI_ACTIVE(true);
 SetbLOAD_ATMOSPHERE(false);
 SetbUSE_TV_PHYSICS(true);
 SetbFULLSCREEN(false); 
 SetRender3DAxis(false);
 SetRenderAtmos(false);
 SetShowFPS(false);
 SetScreensaverEnabled(false);
 SetCEGUIWindowsOpened(false);

}