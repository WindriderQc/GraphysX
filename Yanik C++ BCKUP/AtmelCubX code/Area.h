#pragma once
#include "StdAfx.h"
#include "TV3DMoteur.h"
#include "TV3DCEGUI.h"
#include "Global.h"
#include "3DScenes.h"
#include "Clock.h"

///////////////////
//  This class is used to manage everything that will be rendered in the area, manages all scenes, effect, 2D on scren, physics, etc...
// It also distribute the Mesh to the appropriate scene if it has interaction with IO Devices.
////////////////////////////////////
class CLArea
{
private:
	CLArea(void);
	~CLArea(void);

public:
	void Render(void);
	void Render2DText(void);

	void InitTVScene(void);
	void InitLightAndMaterial(void);
	void InitPhysics(void);
	void Load3DScene(int iSceneID);
	void MeshClickedAction(int iMeshClickedIndex);
	void UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land); //  A metttre dans Scene


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
	CLGlobalVar* clGlobalVar;
	CLTV3DMoteur* clTV3D;
    static CLArea *_singleton;

	CLClock* clClock;

	int iActiveScene;   
	CTVMesh* pCameraAim;
	

public:
	cTV_LIGHT* SoleilLight;
	int iMainLight;

	int iBackground;

	// Declarations des différentes Scene de l'Area
	CLDefaultScene* clDefaultScene;
	CLCarScene* clCarScene;
	CLSkyboxSelectScene* clSkySelectScene;

	


private:
	CTVScene* pScene;
	CTVLightEngine* pLights;
	CTVMaterialFactory* pMatFactory;
	CTVScreen2DText *p2DText;
	CTVGlobals* pGlobals;
	CTVInternalObjects* pIntObj;   //  Ptete inutile maintenant!!!
	CTVPhysics* pTVPhysics;
	CTVMathLibrary* pMath;

	// Fonctions membres
	void Init3DEnvironnement(void);
	void InitCamera(void);

};
