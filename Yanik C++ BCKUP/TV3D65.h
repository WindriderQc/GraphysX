#pragma once
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
//
// CTV3D65 - une classe à instance unique.
//
/////////////////////////////////////////////////////////////////////////////

class CTV3D65
{
private:
  // Constructeur/destructeur
  CTV3D65 () 
		{ 
		tvEngine = new CTVEngine();
		tvInfo = new CTVDeviceInfo();
		tvInput = new CTVInputEngine();
		tvScene = new CTVScene();
		tvLightEngine = new CTVLightEngine();
		tvCamFactory = new CTVCameraFactory();
		tvMath = new CTVMathLibrary();
		tvGlobals = new CTVGlobals();
		tvMaterial = new CTVMaterialFactory();
		tvTexture = new CTVTextureFactory();
		tv2dImmediate = new CTVScreen2DImmediate();
		tv2dText = new CTVScreen2DText();
		tvAtmos = new CTVAtmosphere();
		tvGraphicFX = new CTVGraphicEffect();
		tvGameCtl = new CTVGameControllers();
		tvPhysics = new CTVPhysics();
		tvInternals = new CTVInternalObjects();
		}
  ~CTV3D65 () 
		{
		tvEngine = NULL;
	    tvScene = NULL;
	    tvLightEngine = NULL;
	    tvMath = NULL;
	    tvMaterial = NULL;
	    tvTexture = NULL;
	    tvGlobals = NULL;
	    tvInput = NULL;
	    tv2dImmediate = NULL;
	    tv2dText = NULL;
	    tvAtmos = NULL;
	    tvInfo = NULL;
		}

public:
  // Interface publique
  void setValue (int val) { }
  int getValue () {  }

  // Fonctions de création et destruction du singleton
  static CTV3D65 *getInstance ()
  {
    if (NULL == TV65singleton)
      {
        TV65singleton =  new CTV3D65;
      }
    return TV65singleton;
  }

  static void kill ()
  {
    if (NULL != TV65singleton)
      {
        delete TV65singleton;
        TV65singleton = NULL;
      }
  }


  // Variables membres

		CTVEngine *tvEngine;
        CTVDeviceInfo *tvInfo;
        CTVInputEngine *tvInput;
        CTVScene *tvScene;
        CTVLightEngine *tvLightEngine;
        CTVCameraFactory *tvCamFactory;
        CTVMathLibrary *tvMath;
        CTVGlobals *tvGlobals;
        CTVMaterialFactory *tvMaterial;
        CTVTextureFactory *tvTexture;
        CTVScreen2DImmediate *tv2dImmediate;
        CTVScreen2DText *tv2dText;
        CTVAtmosphere *tvAtmos;
        CTVGraphicEffect *tvGraphicFX;
        CTVGameControllers *tvGameCtl;
        CTVPhysics *tvPhysics;
        CTVInternalObjects *tvInternals;

private:
		static CTV3D65 *TV65singleton;
};

// Initialisation du singleton à NULL
CTV3D65 *CTV3D65::TV65singleton = NULL;




//  Use this to use the engine pointer in any cpp
/*
	// pointeurs sur l'unique instance de la classe CTV3D65
	CTV3D65 *TVMoteur;

	// initialisation des pointeurs
	TVMoteur = CTV3D65::getInstance ();

*/