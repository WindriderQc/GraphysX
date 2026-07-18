#ifndef __Scene_H__
#define __Scene_H__
#pragma once
#include "TV3DMoteur.h"
#include "Camera.h"
#include "Clock.h"
//#include "Newton.h"
#include "dHighResolutionTimer.h"

#define MAX_PHYSICS_LOOPS			1
#define MAX_PHYSICS_FPS				120.0f


class CLSceneStaff
{
public:
	CLTV3DMoteur *clTV3D;
	CLCamera* clCamera;
	CLGlobalVar* clGlobalVar;
	CLClock* clClock;

	CLSceneStaff()
	{
		clGlobalVar = CLGlobalVar::getInstance();
		clTV3D = CLTV3DMoteur::getInstance();
		clCamera = CLCamera::getInstance();
		clClock = CLClock::getInstance();
	};
	~CLSceneStaff()
	{};
};

class CLScene
{
public:
	CLScene();
	~CLScene(void);


	void Render(void);
	void UpdatePhysics(float timestep);
	NewtonWorld* GetWorld() { return(World); }

	CLSceneStaff* clSceneStaff;
protected:
	CTVLightEngine* pTVLightEngine;
	int iMainLight;

	// physics 
	NewtonWorld*	World;
	unsigned64 m_microsecunds;
	bool m_reEntrantUpdate;
	bool m_physicsUpdate;
	dFloat m_physicsTime;
	
};
#endif
