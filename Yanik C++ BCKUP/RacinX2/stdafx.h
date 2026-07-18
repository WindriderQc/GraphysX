// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define HAVE_CONFIG_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <string>
using namespace std;

#define CAR_SCENE	666
#define MODULE_LOADING_SCENE 777
#define NO_SCENE	NULL

#define MAX_PHYSIC_MESH 40

#define FRONT 100
#define BACK  101
#define LEFT  102
#define RIGHT 103
#define UP    104
#define DOWN  105


// TrueVision3D Header Files (For Reference) 
#include "CTVActor.h" 
#include "CTVAI.h" 
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
#include "CTVInternalObjects.h" 
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
#include "CTVPhysics.h" 
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

#define GREEN   RGBA256(0,255,0,255)
#define	BLUE    RGBA256(0,0,255,255)  
#define WHITE   RGBA256(255,255,255,255)
#define BLACK   RGBA256(0,0,0,255)
#define RED		RGBA256(255,0,0,255)
#define YELLOW  RGBA256(0,255,255,255)

// Load LetterAndNumers
	struct ST3DFont
	{
	CTVMesh* Alphabet[26];
	CTVMesh* Numbers[10];
	CTVMesh* SpecialChar[10]; // Pour l'instant:  { . } 
	};


//  rajouter pour la comm série je crois.....
#define _CRT_WARN           0
