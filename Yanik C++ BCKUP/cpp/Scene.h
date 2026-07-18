#pragma once

#ifndef __Scene_H__
#define __Scene_H__

#include "stdafx.h"
#include "TV3DMoteur.h"
#include "Land.h"
#include "Atmosphere.h"
//#include <string>




class CLScene
{
protected:
	CLScene(string PathToLand, string PathToLandTex, string PathToDaySky, string PathToNightSky );
public:	
	virtual ~CLScene(void);
	virtual void Render(long lTimeOfDay) = 0;
	virtual void MeshClickedAction(int iMeshClickedIndex) = 0;
	virtual void ManageCamCtrl(float fWalk, float fStrafe, float fRaise, float fMouseDeltaX, float fMouseDeltaY);
	
protected:
	void InitPhysicsMaterials(void);
	void InitTextures(void);
	void LoadSkyBoxesTextures(void);
	void InitAtmosphere(void);


	CLLand* clLand;
	CLAtmosphere* clAtmos;
	CLWater* clWater;
	
	//Atmos	
	std::string DaySkyPath, NightSkyPath;      //  Possiblement a modifier a cause de la creation de la classe Skybox
	STSkybox stSkyboxDay, stSkyboxNight;
	int pmatTerrain;

	CTVScene* pTVScene;   
	CTVCamera* pTVCamera;		
	CTVMathLibrary* pTVMaths;                 
	CTVTextureFactory* pTVTexturefactory;     
	CTVMaterialFactory* pTVMaterialfactory;                      
	CTVPhysics* pTVPhysics;
	//CLTV3DMoteur* clTV3D;


};

#endif
