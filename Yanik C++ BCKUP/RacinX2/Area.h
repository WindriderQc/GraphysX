#pragma once
#include "StdAfx.h"
#include "TV3DMoteur.h"
#include "TV3DCEGUI.h"
#include "Global.h"
#include "3DScenes.h"


class CLArea
{
private:
	CLArea(void);
	~CLArea(void);

public:
	void RenderArea(float TimeElapsed, long lTimeOfDay);
	void Render2DText(void);

	void InitTVScene(void);
	void InitLightAndMaterial(void);
	void InitPhysics(void);
	void Load3DScene(int iSceneID);

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

	int iActiveScene;   
	CTVMesh* pCameraAim;
	

public:
	cTV_LIGHT* SoleilLight;
	int iMainLight;

	int iBackground;

	// Declarations des différentes Scene de l'Area
	CLScene* clDefaultScene;
	CLCarScene* clCarScene;

	


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
