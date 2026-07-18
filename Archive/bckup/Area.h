#ifndef __AREA_H__
#define __AREA_H__
#pragma once
#include "TV3DMoteur.h"
#include "Global.h"
#include "Camera.h"
#include "Displayer.h"
#include "3DScenes.h"
#include "Input.h"
#include "Clock.h"


///////////////////
//  This class manages all scenes, effect, 2D on screen, physics, etc...
// It also manages interactions between IO Devices and scene objects and renders the GUI.
////////////////////////////////////
class CLArea
{
private:
	CLArea(void);
	~CLArea(void);

public:
	void Render(void);
	void Update(void);

    // Fonctions de création et destruction du singleton
    static CLArea *getInstance ()
  {
    if (NULL == _singleton)
      {
        _singleton =  new CLArea;
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
    // Variables membres
    static CLArea *_singleton;
	CLGlobalVar* clGlobalVar;
	CLTV3DMoteur* clTV3D;
    CLInput* clInput;
	CLDisplayer* clDisplayer;
	CLClock* clClock;

	int iActiveScene; 
	CLScene* pActiveScene;
	cTV_3DVECTOR vTorqueDisplay;
	CTVScene* pScene;
	CTVMaterialFactory* pMatFactory;
	CTVGlobals* pGlobals;

public:
	
	// Declarations des différentes Scene de l'Area
	CLBallZScene* clBallZScene;

};
#endif

