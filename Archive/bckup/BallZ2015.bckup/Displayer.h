#pragma once
#ifndef _DISPLAYER_h__
#define _DISPLAYER_h__

#include "Tools.h"  // FloatToStr
#include "Global.h"
#include "TV3DMoteur.h"
//#include "Input.h"
#include "CLock.h"

/////////////////////////////////////////////////////////////////////////////
//
// 
//
/////////////////////////////////////////////////////////////////////////////

class CLDisplayer
{
private:
  // Constructeur/destructeur
  CLDisplayer ();
  ~CLDisplayer();

	static CLDisplayer *_singleton;
  
    // Load LetterAndNumers
	//struct ST3DFont st3DFont;
  
    bool bShowFPS;
	bool bRender3DAxis;
	CTVScreen2DText *p2DText;
	CTVScene* pTVScene;
	CLGlobalVar* clGlobalVar;
	//CLInput* clInput;
	CLClock* clClock;

	int  IDFont;
	
public:
  // Interface publique
	void Render(float fBumpAmount);
	int iScore, iLap, iTime;

  // Fonctions de création et destruction du singleton
  static CLDisplayer *getInstance ()
  {
    if (NULL == _singleton)
      {
        
        _singleton =  new CLDisplayer;
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
};
#endif