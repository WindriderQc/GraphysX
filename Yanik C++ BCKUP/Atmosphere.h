#pragma once
#include "StdAfx.h"
#include "Clock.h"

#define M_E        2.71828182845904523536
#define M_PI       3.14159265358979323846

#define SUN_ORBIT_RADIUS 1000


class CLAtmosphere
{
public:
	CLAtmosphere(void);
	~CLAtmosphere(void);

	//These indices are store to avoid the lookup costs
    int texDayUp,  texDayDown, texDayLeft, texDayRight, texDayFront, texDayBack;
    int texNightUp,  texNightDown, texNightLeft, texNightRight, texNightFront, texNightBack;
    int texSun;
	int texGround;

	int GetSunOrbitYOffset(void);
	void SetSunOrbitYOffset(int Offset);
	void UpdateAndRender(float TimeElapsed);


	long lTimeOfDay;

//private:
public:
		CTVAtmosphere* pTVAtmos;
		CTVScene* pTV3DScene;
		CTVTextureFactory* pTVTexFactory; 
		CTVLightEngine* pTVLights;
		CTVLandscape* pTVLand;

		CLClock clWorldClock;

		int iIndiceSunlight, iIndiceMoonlight;
		int iSunOrbitYOffset;
     

};
