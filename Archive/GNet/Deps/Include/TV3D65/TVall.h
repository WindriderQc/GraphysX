#pragma once

// TrueVision3D Header Files (For Reference) 
#include "CTVActor.h" 
//#include "CTVAI.h" 
#include "CTVAtmosphere.h" 
//#include "CTVBitmapParts.h" 
#include "CTVCamera.h" 
#include "CTVCameraFactory.h" 
#include "CTVCollisionResult.h" 
#include "CTVDeviceInfo.h" 
#include "CTVEngine.h" 
#include "CTVGameController.h" 
#include "CTVGameControllers.h" 
#include "CTVGlobals.h" 
#include "CTVGraphicEffect.h" 
#include "CTVInputEngine.h" 
#include "CTVInternalObjects.h"     //  DX headers needs to be include in order to use this.
#include "CTVLandscape.h" 
#include "CTVLightEngine.h" 
#include "CTVMaterialFactory.h" 
#include "CTVMathLibrary.h" 
#include "CTVMesh.h" 
#include "CTVMiniMesh.h" 
#include "CTVNode.h" 
//#include "CTVOctree.h" 
#include "CTVParticleSystem.h" 
#include "CTVPath.h" 
//#include "CTVPhysics.h"
#include "CTVRenderSurface.h" 
#include "CTVScene.h" 
#include "CTVScreen2DImmediate.h" 
#include "CTVScreen2DText.h" 
#include "CTVShader.h" 
#include "CTVTextureFactory.h" 
//#include "CTVTileMap.h" 
//#include "CTVViewport.h" 
#include "HelperFunctions.h" 
#include "tv_types.h" 


// Personnal Types  YB2016
#define RGBA_BLACK     RGBA256(  0,   0,   0, 255)
#define RGBA_DARKGRAY  RGBA256( 69,  69,  69, 255)
#define	RGBA_BLUE      RGBA256(  0,   0, 255, 255)  
#define RGBA_AQUA      RGBA256(  0, 255, 255, 255)
#define RGBA_WHITE     RGBA256(255, 255, 255, 255)
#define RGBA_YELLOW    RGBA256(255, 255,   0, 255)
#define RGBA_RED	   RGBA256(255,   0,   0, 255)
#define RGBA_GREEN     RGBA256(  0, 255,   0, 255)

struct sVERTEX
{
	float x; float y; float z;
	sVERTEX() : x(0), y(0), z(0){};
};

struct sLine3D{
	cTV_3DVECTOR Start, End;
	int Color;
	sLine3D() : Start(cTV_3DVECTOR(0, 0, 0)), End(cTV_3DVECTOR(0, 0, 0)), Color(RGBA_WHITE) {};
	sLine3D(cTV_3DVECTOR start, cTV_3DVECTOR end, int color) : Start(start), End(end), Color(color) {};
};

typedef std::vector<cTV_3DVECTOR> Spline;
