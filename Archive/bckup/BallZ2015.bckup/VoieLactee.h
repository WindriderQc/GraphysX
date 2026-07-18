#ifndef __LACTEE_H__
#define __LACTEE_H__
#pragma once
#include "Scene.h"

class CLVoieLactee 
{
public:
	CLVoieLactee(NewtonWorld* World);
	~CLVoieLactee();
	void Update();

private:
	CLSceneStaff* clSceneStaff;
	CL3DObject *clEarth, *clEarthClouds, *clMars, *clMoon, *clVenus;
	int iEarthTex, iEarthNightTex, iEarthCloudsTex, iMarsTex, iMoonTex, iVenusTex;

};
#endif
