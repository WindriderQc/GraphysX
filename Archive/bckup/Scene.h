#ifndef __Scene_H__
#define __Scene_H__
#pragma once
#include "TV3DMoteur.h"
#include "Camera.h"
#include "3DObjMgr.h"
#include "Input.h"
#include "Displayer.h"
#include "Sound.h"


class CLSceneTools
{
public:
	CLTV3DMoteur *clTV3D;
	CLInput* clInput;
	CLDisplayer* clDisplayer;
	CLSound* clSound;
	CL3DObjMgr *cl3DObjMgr;
	CLCamera* clCamera;
	CLGlobalVar* clGlobalVar;

	CLSceneTools()
	{
		clGlobalVar = CLGlobalVar::getInstance();
		clTV3D = CLTV3DMoteur::getInstance();
		clInput = CLInput::getInstance();
		cl3DObjMgr = CL3DObjMgr::getInstance();
		clDisplayer = CLDisplayer::getInstance();
		clSound = CLSound::getInstance();
		clCamera = CLCamera::getInstance();
	};
	~CLSceneTools()
	{};

};

class CLScene : public CLSceneTools
{
protected:
	CLScene();
	~CLScene(void);
public:	
	virtual void Render(long lTimeOfDay) = 0;
	

protected:
	void InitPhysicsMaterials(void);
	
	NewtonWorld*	World;
	CTVLightEngine* pTVLightEngine;
	int iMainLight;
	
};

#endif
