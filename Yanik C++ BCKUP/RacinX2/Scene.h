#pragma once

#ifndef __Scene_H__
#define __Scene_H__


#include "stdafx.h"
#include "Land.h"
#include "Atmosphere.h"
#include <string>


class CLScene
{
public:

	CLScene(string PathToLand, string PathToLandTex, string PathToDaySky, string PathToNightSky );
	virtual ~CLScene(void);

	virtual void Render(float TimeElapsed, long lTimeOfDay);
	void InitPhysicsMaterials(void);
	void InitTextures(void);
	void LoadSkyBoxesTextures(void);
	void InitAtmosphere(void);
	long GetSceneTimeOfDay(void);

protected:
	
	CLLand* clLand;
	CLAtmosphere* clAtmos;
	CLWater* clWater;
	
	//Atmos	
	std::string DaySkyPath, NightSkyPath;
	STSkybox stSkyboxDay, stSkyboxNight;
	int pmatTerrain;
			
	CTVScene* pTVScene;   
	CTVCamera* pTVCamera;		
	CTVMathLibrary* pTVMaths;                 
	CTVTextureFactory* pTVTexturefactory;     
	CTVMaterialFactory* pTVMaterialfactory;                      
	CTVPhysics* pTVPhysics;

};

#endif
