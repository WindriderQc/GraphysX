#pragma once

#ifndef __Scene_H__
#define __Scene_H__


#include "stdafx.h"
#include "Land.h"
#include "Atmosphere.h"
#include <string>


class CLScene
{
protected:

	CLScene(string PathToLand, string PathToLandTex, string PathToDaySky, string PathToNightSky );
	virtual ~CLScene(void);

	virtual void Render(long lTimeOfDay);
	
	

protected:
	void InitPhysicsMaterials(void);
	void InitTextures(void);
	void LoadSkyBoxesTextures(void);
	void InitAtmosphere(void);


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
