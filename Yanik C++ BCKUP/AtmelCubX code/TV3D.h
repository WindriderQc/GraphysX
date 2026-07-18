//  TV3D.h

// Defines some global ID tu be used within TV3d
// Includes all needed Headers for TV3D

#define CAR_SCENE	666
#define MODULE_LOADING_SCENE 777
#define SKYBOX_SELECT_SCENE 888
#define NO_SCENE	NULL

#define MAX_PHYSIC_MESH 40

#define FRONT 100
#define BACK  101
#define LEFT  102
#define RIGHT 103
#define UP    104
#define DOWN  105

#define GREEN   RGBA256(0,255,0,255)
#define	BLUE    RGBA256(0,0,255,255)  
#define WHITE   RGBA256(255,255,255,255)
#define BLACK   RGBA256(0,0,0,255)
#define RED		RGBA256(255,0,0,255)
#define YELLOW  RGBA256(0,255,255,255)


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



// Load LetterAndNumers  
// Devrait ptete etre mis ailleur
	struct ST3DFont
	{
	CTVMesh* Alphabet[26];
	CTVMesh* Numbers[10];
	CTVMesh* SpecialChar[10]; // Pour l'instant:  { . } 
	};