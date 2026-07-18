#pragma once
#include "StdAfx.h"
#include "Level.h"
#include "Ball.h"

class CLBallZ
{
public:
	CLBallZ(void);
public:
	~CLBallZ(void);

	void Render(void);
	void PushBall(int iDir);
	void InitPhysicsMaterials(void);
	CTVMesh* GetBallMesh(void);
	int GetBallPhysicBody(void);
	int GetSlidePhysicBody(void);
	cTV_3DVECTOR GetBallPosition(void);
	cTV_3DVECTOR GetBallSpeed(void);

private:
	CTVPhysics* pTVPhysics;
	CLLevel* clLevel;
	CLBall* clBall;
};
