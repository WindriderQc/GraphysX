#ifndef __ATMOSPHERE_H__
#define __ATMOSPHERE_H__
#pragma once
#include "Global.h"
#include "TV3DMoteur.h"
#define MILLISEC_PER_DAY 86400000
#define SUN_ORBIT_RADIUS 10000
#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846


struct STSkybox
{
	int SkyTop;
	int SkyBottom; 
	int SkyLeft; 
	int SkyRight;
	int SkyFront;
	int SkyBack; 
};


class CLAtmosphere
{
public:
	CLAtmosphere(STSkybox stSkyBoxDay, STSkybox stSkyboxNight);
	~CLAtmosphere(void);

	CLGlobalVar* clGlobalVar;

	//These indices are store to avoid the lookup costs
    int texDayUp,  texDayDown, texDayLeft, texDayRight, texDayFront, texDayBack;
    int texNightUp,  texNightDown, texNightLeft, texNightRight, texNightFront, texNightBack;
    int texSun;

	STSkybox stSkyDay, stSkyNight;

	int GetSunOrbitYOffset(void);
	void SetSunOrbitYOffset(int Offset);
	void UpdateAndRender(long lTimeOfDay, float fFixedAlpha = -1);

//private:
public:
		CTVAtmosphere* pTVAtmos;
		CTVTextureFactory* pTVTexFactory; 
		CTVLightEngine* pTVLights;
		//CTVLandscape* pTVLand;
		
		int iIndiceSunlight, iIndiceMoonlight;
		int iSunOrbitYOffset;  
		//int pNewtonLand;
};


class CLWater
{
public:
	CLWater(void);
	~CLWater(void);

		void StartReflectRender(void);
		void StopReflectRender(void);
		void StartRefractRender(void);
		void StopRefractRender(void);
		void Render(void);
		CTVRenderSurface* pReflectRS;   // sera copier sur d'autre surface au besoin, question d'optimiser le frame rate 
protected:
		CTVScene* pTVScene;
		CTVTextureFactory* pTVTexFactory; 

		CTVGraphicEffect* pTVGraphEffect;
		 
		CTVRenderSurface* pRefractRS;
		CTVMesh* pWaterMesh;
		cTV_PLANE* pWaterPlane;

		float WaterHeight;

};


class CLLand
{
public:
	CLLand(string HeightmapPath, string TexturePath, float fWidth, float fHeight, cTV_3DVECTOR Pos, const NewtonWorld* World);
	~CLLand(void);

	void Render(void);
	void SetPos(cTV_3DVECTOR pos);
	//NewtonBody* LandPhysicBody;

private:
	CTVLandscape* pTVLand;

	//NewtonCollision * Collision;
	//NewtonMatrix* NewtonMX;
	//CTVPhysics* pTVPhysics;
	CTVScene* pTVScene;
	CTVTextureFactory* pTVTexFactory; 
	CTVMaterialFactory* pTVMaterialfactory;

	int matLand;
	
	int texGround;

};
#endif