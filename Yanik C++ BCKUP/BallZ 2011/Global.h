#pragma once
#include "stdafx.h"

 
/////////////////////////////////////////////////////////////////////////////
//
// CLGlobalVar - une classe à instance unique.
// with all Global variable to contain configuration parameters
//
/////////////////////////////////////////////////////////////////////////////

class CLGlobalVar
{
private:
  // Constructeur/destructeur
  CLGlobalVar ();
  ~CLGlobalVar () { }

public:
  // Interface publique
  void SetShowFPS(bool bState)			   { bShowFPS = bState; }
  void SetRender3DAxis(bool bState)		   { bRender3DAxis = bState; }
  void SetRenderAtmos(bool bState)         { bRenderAtmos = bState; }
  void SetScreensaverEnabled(bool bState)  { bScreensaverEnabled = bState; }
  void SetScreensaverRunning(bool bState)  { bScreensaverRunning = bState; }
  void SetCEGUIWindowsOpened(bool bState)  { bCEGUIWindowsOpened = bState; }
  void SetbFULLSCREEN(bool bState)		   { bFULLSCREEN = bState; }
  void SetbDEBUG_MODE (bool bState)		   { bDEBUG_MODE = bState; }
  void SetbMOUSE_CAM_CTRL (bool bState)	   { bMOUSE_CAM_CTRL = bState; }
  void SetbCAM_CTRL (bool bState)		   { bCAM_CTRL = bState; }
  void SetbCEGUI_ACTIVE (bool bState)      { bCEGUI_ACTIVE = bState; }
  void SetbLOAD_ATMOSPHERE (bool bState)   { bLOAD_ATMOSPHERE = bState; }
  void SetbUSE_TV_PHYSICS (bool bState)    { bUSE_TV_PHYSICS = bState; }
  void SetbCAM_FOLLOW_CAR (bool bState)    { bCAM_FOLLOW_CAR = bState; }
  void SetbTVSHADOWS (bool bState)		   { bTVSHADOWS = bState; }
  void SetbENABLE_LAND (bool bState)	   { bENABLE_LAND = bState; }
  void Set3DWindowHandle (HWND handle)     { w3DWindowHandle = handle; }
  void SetSearchPath (char *path)		   { SearchPath = path; }
  void SetActiveScene (int iSceneID)       { iActiveScene = iSceneID; }
  void SetTorqueValue(cTV_3DVECTOR vTorque)  { Torq = vTorque;}

  bool GetShowFPS()			   { return bShowFPS; }
  bool GetRender3DAxis()	   { return bRender3DAxis; }
  bool GetRenderAtmos()		   { return bRenderAtmos; }
  bool IsScreensaverEnabled()  { return bScreensaverEnabled; }
  bool IsScreensaverRunning()  { return bScreensaverRunning; }
  bool IsCEGUIWindowsOpened()  { return bCEGUIWindowsOpened; }
  bool GetbFULLSCREEN()		   { return bFULLSCREEN; }
  bool GetbDEBUG_MODE()		   { return bDEBUG_MODE; }
  bool GetbMOUSE_CAM_CTRL()	   { return bMOUSE_CAM_CTRL; }
  bool GetbCAM_CTRL()		   { return bCAM_CTRL; }
  bool IsCEGUIEnabled()        { return bCEGUI_ACTIVE; }
  bool GetbLOAD_ATMOSPHERE()   { return bLOAD_ATMOSPHERE; }
  bool GetbUSE_TV_PHYSICS()    { return bUSE_TV_PHYSICS; }
  bool GetbCAM_FOLLOW_CAR()    { return bCAM_FOLLOW_CAR; }
  bool GetbTVSHADOWS()		   { return bTVSHADOWS; }
  bool GetbENABLE_LAND()	   { return bENABLE_LAND; }
  HWND Get3DWindowHandle()     { return w3DWindowHandle; }
  char* GetSearchPath()		   { return SearchPath; }
  int GetActiveScene()		   { return iActiveScene; }
  cTV_3DVECTOR GetTorqueValue()  { return Torq; }

  // Fonctions de création et destruction du singleton
  static CLGlobalVar *getInstance ()
  {
    if (NULL == _singleton)
      {
        // std::cout << "creating singleton." << std::endl;
        _singleton =  new CLGlobalVar;
      }
    else
      {
		// std::cout << "singleton already created!" << std::endl;
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
    static CLGlobalVar *_singleton;

    bool bShowFPS;
	bool bRender3DAxis;
	bool bRenderAtmos;
	bool bScreensaverEnabled;
	bool bScreensaverRunning;
	bool bCEGUIWindowsOpened; 
	bool bFULLSCREEN; 
	bool bDEBUG_MODE; 
	bool bMOUSE_CAM_CTRL; 
	bool bCAM_CTRL; 
	bool bCEGUI_ACTIVE; 
	bool bLOAD_ATMOSPHERE; 
	bool bUSE_TV_PHYSICS; 
	bool bCAM_FOLLOW_CAR; 
	bool bTVSHADOWS; 
	bool bENABLE_LAND; 

	HWND w3DWindowHandle;
	char* SearchPath;
	int iActiveScene;
	cTV_3DVECTOR Torq;

};