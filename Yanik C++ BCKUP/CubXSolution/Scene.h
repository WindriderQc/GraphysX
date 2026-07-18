#pragma once

#ifndef __Scene_H__
#define __Scene_H__

#include "stdafx.h"
#include <math.h>
#include "Land.h"
#include "Water.h"
#include "CLSkybox.h"

class CLScene
{
protected:
	CLScene(string PathToLand, string PathToLandTex);
public:	
	virtual ~CLScene(void);
	virtual void Render(long lTimeOfDay) = 0;
	
	virtual void UpdateScene(void) = 0;
	virtual void MeshClickedAction(int iMeshClickedIndex) = 0;
	virtual void ManageCamCtrl(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY, float fTimeElapsed);
	
	void SetSkybox(CLSkybox* clSky);
	void UpdateWater(long lTimeOfDay);

protected:
	void InitPhysicsMaterials(void);
	void InitTextures(void);
	void LoadSkyBoxesTextures(void);
	void InitAtmosphere(void);
	cTV_3DVECTOR GenerateCerclePos(int Index, int MaxUnit, float Radius);

	CLSkybox* clSkybox;
	CLLand* clLand;
	CLWater* clWater;

	CTVMesh* mCameraAim;
	
	// Physics Land Material
	int pmatTerrain;

	CTVScene* pTVScene;   
	CTVCamera* pTVCamera;		
	CTVMathLibrary* pTVMaths;                
	CTVTextureFactory* pTVTexturefactory;     
	CTVMaterialFactory* pTVMaterialfactory;                      
	CTVPhysics* pTVPhysics;
	CTVAtmosphere* pTVAtmos;
};

#endif
