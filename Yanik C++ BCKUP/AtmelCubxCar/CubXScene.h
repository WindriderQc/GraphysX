#pragma once
#include "StdAfx.h"
#include "TV3DMoteur.h"
#include "3DObject.h"
#include "CubX.h"
#include "Land.h"
#include "ClockDisplay.h"
#include "MenuManager.h"
#include "Effect.h"


class CLCubXScene
{
public:
	CLCubXScene(void);
	~CLCubXScene(void);

	void Render(void);
	void InitTextures(void);
	void CreateMenuDomotique(void);
	void ShowTimeDisplays(void);
	void HideTimeDisplays(void);
	void Refresh3DClock(void);
	void SetMenuButtons(void);
	void CubXClickedAction(int iMeshClickedIndex);
	void DomoticClickedAction(int iMeshClickedIndex);
	void LoadLettersAndNumbers(void);
	void WriteString(char *cMot, double size, double x, double y, double z, double angle_y, int Color);
	
	CLCubX* clCubX;
	CLLand* clLand;

	CL3DObject* clEarth; 
	CL3DObject* clEarthGrid; 
	CL3DObject* clCubeSys; 
	CL3DObject* clCubeTools; 
	CTVMesh* pSoleil;
	CL3DObject* clFleche; 
	
	CLMenuManager* clMenuManager;
	CLEffect* clEffect;

private:
		CLTV3DMoteur* clTV3D;
		CTVScene* pTVScene;  
		CTVTextureFactory* pTexFactory; 
		
		// Load LetterAndNumers
		struct ST3DFont st3DFont;

	CL3DObject* clMenuDomotique;
	CL3DObject* clInetConnected;
	CTVMesh* pIOButton[8];
	////  Gestion de serre ////
	cTV_3DVECTOR DisplayPos;
	CLClockDisplay *ClockDisplay;
	CLClockDisplay *LightTimeOn;
	CLClockDisplay *LightTimeOff;
	CLClockDisplay *FanTimeOn;
	CLClockDisplay *FanTimeOff;

	CL3DObject* clLightEtat;
	CL3DObject* clFanEtat;
	//////////////////////////

	// Textures
	int TexBSystem, TexBTools, ClockTexture, MarbleTexture, EarthTexture, EarthGridTex;
/*		
	int	CubeTexture, SmokeTexture, SnowTexture,  TempSensTexture, CursorTexture,  PlasticTexture, CloudsTexture;
	int TexturePanneauMenu,TexBAlarms,TexBAtmel, TexBTemp,  CubesTex[3], SoleilTex;
	int ClosedIconTex, OpenedIconTex;
*/
};
