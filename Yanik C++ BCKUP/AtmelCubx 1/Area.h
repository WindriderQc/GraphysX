#pragma once
#include "StdAfx.h"
#include "TV3DMoteur.h"
#include "Atmosphere.h"
#include "Land.h"
#include "CL_TV3DCEGUI.h"
#include "MenuManager.h"
#include "ParticleEng.h"
#include "ClockDisplay.h"
#include "Explosion.h"
#include "BallZ.h"
#include "CubX.h"
#include "3DObject.h"
#include "Animation.h"
#include "CarScene.h"
#include "Global.h"
#include "NotesManager.h"

//  A verifier, ca ne devrais ptete pas etre un singleton...   car plusieurs type d'area possible


#define CAR_MODE   1001
#define CUBX_MODE  1002
#define BALLZ_MODE 1003


class CLArea
{
private:
	CLArea(void);
	~CLArea(void);

public:
	void RenderArea(void);
	void Render4CubX(float TimeElapsed, bool bScreensaverRunning);
	void Render4CarMode(float TimeElapsed);
	void Render4BallZMode(float TimeElapsed);
	void Render2DText(void);

	

	void InitLightAndMaterial(void);
	void InitPhysics(void);
	void Create3DObjects(void);
	void CreateMenuDomotique(void);
	void ShowTimeDisplays(void);
	void HideTimeDisplays(void);
	void Refresh3DClock(void);
	void SetMenuButtons(void);
	void LoadTextures(void);
	void LoadLettersAndNumbers(void);
	void MeshClickedAction(int iMeshClickedIndex);
	void WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color);
	
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



	
	CL3DObject* clEarth; 
	CL3DObject* clEarthGrid; 
	CL3DObject* clCubeSys; 
	CL3DObject* clCubeTools; 
	CL3DObject* clFleche; 
	CL3DObject* clMenuDomotique;
	CL3DObject* clInetConnected;
	
	

	// Load LetterAndNumers
	struct ST3DFont st3DFont;

	cTV_3DVECTOR DisplayPos;
	////  Gestion de serre ////
	CLClockDisplay *ClockDisplay;
	CLClockDisplay *LightTimeOn;
	CLClockDisplay *LightTimeOff;
	CLClockDisplay *FanTimeOn;
	CLClockDisplay *FanTimeOff;

	CL3DObject* clLightEtat;
	CL3DObject* clFanEtat;
	//////////////////////////


	CTVMesh* pBille;
	CTVMesh* pGround;
	CTVMesh* pCameraAim;
	CTVMesh* pSoleil;
	CTVMesh* pIOButton[8]; 
	CTVMesh* pClouds; 
	CTVMesh* pMeshClicked;
	CTVMesh* pConfigBtn;
	

	// Textures
	int CubeTexture, SmokeTexture, SnowTexture, ClockTexture, TempSensTexture, CursorTexture, MarbleTexture, EarthTexture, PlasticTexture, CloudsTexture;
	int TexturePanneauMenu,TexBAlarms,TexBAtmel, TexBSystem, TexBTemp, TexBTools, CubesTex[3],EarthGridTex, SoleilTex;
	int ClosedIconTex, OpenedIconTex;

	
	CLAtmosphere* clAtmos;
	CLWater* clWater;
	CLLand* clLand;

public:
	CLMenuManager* clMenuManager;
	TSnowfall* Snowfall;
	CLSmoke* SmokeFest;
	CLExplosion* clExplosion;
	CLBallZ* clBallZ;
	CLCarScene* clCarScene;
	CLCubX* clCubX;
	CLNotesManager* clNotesManager;

	cTV_LIGHT* SoleilLight;
	int iMainLight;



	CTVScene* pScene;
	
	//CTVInputEngine* pInput;
	CTVTextureFactory* pTexFactory; 
	CTVMaterialFactory* pMatFactory;
	
	CTVScreen2DText *p2DText;
	CTVGlobals* pGlobals;
	CTVLightEngine* pLights;

	CTVInternalObjects* pIntObj;
	CTVPhysics* pTVPhysics;

	// Fonctions membres

	void Init3DEnvironnement(void);
	void InitScene(void);
	void InitCamera(void);

};
