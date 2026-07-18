#pragma once
#include "StdAfx.h"
#include "TV3DMoteur.h"
#include "Atmosphere.h"
#include "TV3DCEGUI.h"
#include "BallZ.h"
#include "Global.h"
#include "NotesManager.h"
#include "CubXScene.h"
#include "CarScene.h"
#include "FlightXScene.h"


// Area Mode
#define CAR_MODE      1001
#define CUBX_MODE     1002
#define BALLZ_MODE    1003
#define FLIGHTX_MODE  1004
#define NOTEMGR_MODE  1005



class CLArea
{
private:
	CLArea(void);
	~CLArea(void);

public:
	void RenderArea(float TimeElapsed);
	void Render2DText(void);

	
	void InitTVScene(void);
	void InitLightAndMaterial(void);
	void InitPhysics(void);
	void MeshClickedAction(int iMeshClickedIndex);
	void SelectScene(int iAreaMode);
	void UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land);
	
	int iSelectedArea;


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

	CTVMesh* pCameraAim;
	
	CLAtmosphere* clAtmos;
	CLWater* clWater;

public:
	CLBallZ* clBallZ;

	cTV_LIGHT* SoleilLight;
	int iMainLight;
	
	// Declarations des différentes Scene de l'Area
	CLCubXScene* clCubXScene;
	CLCarScene* clCarScene;
	CLFlightXScene* clFlightXScene;

	CLNotesManager* clNotesManager;

private:
	CTVScene* pScene;
	CTVLightEngine* pLights;
	CTVMaterialFactory* pMatFactory;
	CTVScreen2DText *p2DText;
	CTVGlobals* pGlobals;
	CTVInternalObjects* pIntObj;
	CTVPhysics* pTVPhysics;
	CTVMathLibrary* pMath;

	// Fonctions membres
	void Init3DEnvironnement(void);
	void InitCamera(void);

};
