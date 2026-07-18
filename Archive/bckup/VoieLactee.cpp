#include "stdafx.h"
#include "VoieLactee.h"


CLVoieLactee::CLVoieLactee(NewtonWorld* World)
{
	iEarthTex = clTV3D->pTVTexturefactory->LoadTexture("media\\Galaxy\\Earth.jpg", "Earth");
	iEarthNightTex = clTV3D->pTVTexturefactory->LoadTexture("media\\Galaxy\\EarthNight.jpg", "EarthNight");
	iEarthCloudsTex = clTV3D->pTVTexturefactory->LoadTexture("media\\Galaxy\\EarthCloudsMapHigh.png", "EarthClouds", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);
	iMarsTex = clTV3D->pTVTexturefactory->LoadTexture("media\\Galaxy\\MarsMap.jpg", "Mars");
	iMoonTex = clTV3D->pTVTexturefactory->LoadTexture("media\\Galaxy\\MoonMap.jpg", "Moon");
	iVenusTex = clTV3D->pTVTexturefactory->LoadTexture("media\\Galaxy\\VenusMap.jpg", "Venus");

	//EARTH
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(SPHERE, "Earth", NULL, iEarthTex, cTV_3DVECTOR(-10, 15, 0), cTV_3DVECTOR(6.0f, 6.0f, 6.0f), true, NULL, NULL, NULL, World));
	clEarth = cl3DObjMgr->VecObjects.back();
	//clEarth->Mesh->RotateX(23.0f);
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(SPHERE, "EarthClouds", NULL, iEarthCloudsTex, cTV_3DVECTOR(-10, 15, 0), cTV_3DVECTOR(6.1f, 6.1f, 6.1f), true, NULL, NULL, NULL, World));
	clEarthClouds = cl3DObjMgr->VecObjects.back();
	clEarthClouds->Mesh->SetTexture(iEarthCloudsTex);
	clEarthClouds->Mesh->SetAlphaTest(true, 0);
	clEarthClouds->Mesh->SetBlendingMode(cTV_BLEND_ALPHA);
	//clEarthClouds->Mesh->SetLightingMode(cTV_LIGHTING_NORMAL);
	//clEarthClouds->SetTranslucentMaterial();


	//MOON
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(SPHERE, "Moon", NULL, iMoonTex, cTV_3DVECTOR(-8, 14, -10), cTV_3DVECTOR(2.0f, 2.0f, 2.0f), true, NULL, NULL, NULL, World));
	clMoon = cl3DObjMgr->VecObjects.back();
	

	
	//MARS
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(SPHERE, "Mars", NULL, iMarsTex, cTV_3DVECTOR(-25, 15, 0), cTV_3DVECTOR(12.0f, 12.0f, 12.0f), true, NULL, NULL, NULL, World));
	clMars = cl3DObjMgr->VecObjects.back();

	//VENUS
	cl3DObjMgr->VecObjects.push_back(new CL3DObject(SPHERE, "Venus", NULL, iVenusTex, cTV_3DVECTOR(-35, 15, 0), cTV_3DVECTOR(4.0f, 4.0f, 4.0f), true, NULL, NULL, NULL, World));
	clMars = cl3DObjMgr->VecObjects.back();


}


CLVoieLactee::~CLVoieLactee()
{

	//  DOIT DETRUIRE LES clOBJECT
}

void CLVoieLactee::Update()
{
	clEarth->Rotate(cTV_3DVECTOR(0, 1, 0), 0.02f);
	clEarthClouds->Rotate(cTV_3DVECTOR(0, 1, 0), 0.06f);
	clMoon->RotateAround( -0.003f * clGlobalVar->fTimeElapsed, &clEarth->Mesh->GetPosition(), 8.0f);
}