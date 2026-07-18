#pragma once
#ifndef _GLOBAL_h__
#define _GLOBAL_h__

/////////////////////////////////////////////////////////////////////////////
//
// CLGlobalVar - une classe à instance unique.
// with all Global variable to contain configuration parameters and Status
//
/////////////////////////////////////////////////////////////////////////////

class CLGlobalVar
{
private:
	CLGlobalVar();
   ~CLGlobalVar();
	static CLGlobalVar *_singleton;

public:
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

	float fBallSpeed;
	float fDayNightAlpha;
	bool bRender3DAxis;
	bool bFULLSCREEN; 
	bool bDEBUG_MODE; 
	bool bMOUSE_CAM_CTRL; 
	HWND w3DWindowHandle;
	char* SearchPath;
	float fTimeElapsed;

};
#endif