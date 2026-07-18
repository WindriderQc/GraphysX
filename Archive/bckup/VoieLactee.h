#ifndef __LACTEE_H__
#define __LACTEE_H__
#pragma once
#include "Scene.h"

class CLVoieLactee : private CLSceneTools
{
public:
	CLVoieLactee(NewtonWorld* World);
	~CLVoieLactee();
	void Update();

private:
	CL3DObject *clEarth, *clEarthClouds, *clMars, *clMoon, *clVenus;
	int iEarthTex, iEarthNightTex, iEarthCloudsTex, iMarsTex, iMoonTex, iVenusTex;

};
#endif
