#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////
//
// UniqueObject - une classe à instance unique.
//
/////////////////////////////////////////////////////////////////////////////

class UniqueObject
{
private:
  // Constructeur/destructeur
  UniqueObject () 
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
  ~UniqueObject () 
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
  static UniqueObject *getInstance ()
  {
    if (NULL == _singleton)
      {
       // std::cout << "creating singleton." << std::endl;
        _singleton =  new UniqueObject;
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

  static UniqueObject *_singleton;
};

// Initialisation du singleton à NULL
UniqueObject *UniqueObject::_singleton = NULL;