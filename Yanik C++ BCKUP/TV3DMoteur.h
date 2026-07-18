#pragma once
#include "stdafx.h"
#include "Input.h"
#include "Atmosphere.h"
#include "CL_TV3DCEGUI.h"
#include "MenuManager.h"
#include "ParticleEng.h"
#include "ClockDisplay.h"
#include "Explosion.h"
#include "BallZ.h"





class CLTV3DMoteur
{
public:
	//Constructeur de la classe
	CLTV3DMoteur(void);
	//Destructeur de la classe
	~CLTV3DMoteur(void);
	
	void Render(float TimeElapsed);
	void Init3D(HWND WindowHandle, char *srchpath);
	void InitLight(void);
	void Create3DObjects(void);
	void CreateMenuDomotique(void);
	void ShowTimeDisplays(void);
	void HideTimeDisplays(void);
	void Refresh3DClock(void);
	void SetCamera(void);
	void DrawTV3DAxis(void);
	void DrawMeshAxis(CTVMesh* mesh);
	void SetMenuButtons(void);
	void LoadTextures(void);
	float GetTimeElapsed(void);
	UINT InputAction(void);
	void GetMeshClicked(void);
	void MeshClickedAction(void);
	void LoadLettersAndNumbers(void);
	void WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color);

protected:		
	//Setup TrueVision3D
	// TV3D Variables:
	CTVEngine* pTV;
	CTVScene* pScene;
	CTVInputEngine* pInput;
	CTVTextureFactory* pTexFactory; 
	CTVScreen2DImmediate* p2DImmediate;
	CTVScreen2DText *p2DText;
	CTVGlobals* pGlobals;
	CTVLightEngine* pLights;
	CTVCollisionResult* pCollision;
	CTVInternalObjects* pIntObj;


public:
	cTV_3DVECTOR CamPos; 
	cTV_3DVECTOR CamLookAt;
	
	int RED,BLUE,BLACK,WHITE,GREEN;

	cTV_LIGHT* SoleilLight;

	//  pour la physique
	CTVMesh* MeshTable[MAX_PHYSIC_MESH];  
	CTVMathLibrary* pTVMath;

	CTVMesh *pBille;
	CTVMesh *pGround;

	CTVMesh *pSoleil;

	CTVMesh* pCube[3]; CTVMesh* pIOButton[8]; CTVMesh* pMenuDomotique;
	CTVMesh* pBoite;CTVMesh* pSensTempBboard;CTVMesh* pGear;CTVMesh* pEarth;CTVMesh* pEarthGrid;CTVMesh* pSwitch;CTVMesh* pClouds;CTVMesh* pMeshClicked;CTVMesh* pFleche;
	CTVMesh* pInetConnected; CTVMesh* pConfigBtn;
	

	// Textures
	int CubeTexture, SmokeTexture, SnowTexture, ClockTexture, TempSensTexture, CursorTexture, MarbleTexture, EarthTexture, PlasticTexture, CloudsTexture;
	int TexturePanneauMenu,TexBAlarms,TexBAtmel, TexBSystem, TexBTemp, TexBTools, CubesTex[3],EarthGridTex, SoleilTex;
	
	int ClosedIconTex, OpenedIconTex;



	CInput* clInput;
	CLAtmosphere* clAtmos;
	CL_TV3DCEGUI* clCEGUI;
	CLMenuManager* clMenuManager;
	TSnowfall* Snowfall;
	CLSmoke* SmokeFest;
	CLExplosion* clExplosion;

	CLBallZ* clBallZ;


	// Load LetterAndNumers
	struct ST3DFont st3DFont;


	cTV_3DVECTOR DisplayPos;
	////  Gestion de serre ////
	CLClockDisplay *ClockDisplay;
	CLClockDisplay *LightTimeOn;
	CLClockDisplay *LightTimeOff;
	CLClockDisplay *FanTimeOn;
	CLClockDisplay *FanTimeOff;

	CTVMesh* LightEtat;
	CTVMesh* FanEtat;

	//////////////////////////


	UINT uiAction;

	bool bFULLSCREEN;
	bool bDEBUG_MODE;
	bool bMOUSE_CAM_CTRL;
	bool bCAM_CTRL;
	bool bCAM_FOLLOW_CAR;
	bool bTVSHADOWS;
	bool bCEGUI_ACTIVE;
	bool bSHOW_FPS;
	bool bDRAW_AXIS;
	bool bENABLE_ATMOSPHERE;
	bool bGAME_MODE;
};