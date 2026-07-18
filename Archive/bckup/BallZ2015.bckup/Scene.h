#ifndef __Scene_H__
#define __Scene_H__
#pragma once
#include "TV3DMoteur.h"
#include "Camera.h"
#include "3DObjMgr.h"
#include "Input.h"
#include "Displayer.h"
#include "Sound.h"
#include "Clock.h"
#include "dHighResolutionTimer.h"

class CLSceneStaff
{
public:
	CLTV3DMoteur *clTV3D;
	CLInput* clInput;
	CLDisplayer* clDisplayer;
	CLSound* clSound;
	CL3DObjMgr *cl3DObjMgr;
	CLCamera* clCamera;
	CLGlobalVar* clGlobalVar;
	CLClock* clClock;

	CLSceneStaff()
	{
		clGlobalVar = CLGlobalVar::getInstance();
		clTV3D = CLTV3DMoteur::getInstance();
		clInput = CLInput::getInstance();
		cl3DObjMgr = CL3DObjMgr::getInstance();
		clDisplayer = CLDisplayer::getInstance();
		clSound = CLSound::getInstance();
		clCamera = CLCamera::getInstance();
		clClock = CLClock::getInstance();
	};
	~CLSceneStaff()
	{};
};

class CLScene
{
protected:
	CLScene();
	~CLScene(void);

public:
	virtual void Render(void) = 0;
	NewtonWorld* GetWorld(){ return(World); }
	int GetDefaultPhysMaterial(){ return (defaultPhysMatID); }

	CLSceneStaff* clSceneStaff;
protected:
	NewtonWorld*	World;
	CTVLightEngine* pTVLightEngine;
	int iMainLight;
	int defaultPhysMatID;

	//update physics variable
	unsigned64 m_microsecunds;
	bool m_reEntrantUpdate;
	bool m_physicsUpdate;
	dFloat m_physicsTime;

	void UpdatePhysics(float timestep);
};
#endif
