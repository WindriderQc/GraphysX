#pragma once
#include "StdAfx.h"
#include "TV3DMoteur.h"
#include "TV3DCEGUI.h"
#include "Global.h"
#include "3DScenes.h"
#include "Clock.h"

///////////////////
//  This class manages everything that will be rendered in the area, manages all scenes, effect, 2D on screen, physics, etc...
// It also manages interactions between IO Devices and scene objects and renders the GUI.
////////////////////////////////////
class CLArea
{
private:
	CLArea(void);
	~CLArea(void);

public:
	void Render(void);
	void Update(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY);
	void Render2DText(void);

	void InitTVScene(void);
	void InitLightAndMaterial(void);
	void InitPhysics(void);
	void Load3DScene(int iSceneID);
	void SendMeshClickedToScene(int iMeshClickedIndex);
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
	CLScene* pActiveScene;
	CTVMesh* pCameraAim;
	

public:
	cTV_LIGHT* SoleilLight;
	int iMainLight;

	int iBackground;

	// Declarations des différentes Scene de l'Area
	CLDefaultScene* clDefaultScene;
	CLCarScene* clCarScene;
	CLSkyboxSelectScene* clSkySelectScene;
	CLCarSelectScene* clCarSelectScene;

	


private:
	CTVScene* pScene;
	CTVLightEngine* pLights;
	CTVMaterialFactory* pMatFactory;
	CTVScreen2DText *p2DText;
	CTVGlobals* pGlobals;
	CTVInternalObjects* pIntObj;   //  Ptete inutile maintenant!!!
	CTVPhysics* pTVPhysics;
	

	// Fonctions membres
	void Init3DEnvironnement(void);
	void InitCamera(void);

};
