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
	CTVMesh* GetBallMesh(void);

private:
	CLLevel* clLevel;
	CLBall* clBall;
};
