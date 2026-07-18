#pragma once

#include "StdAfx.h"


class CLExplosion
{
public:
	CLExplosion(void);
	~CLExplosion(void);

//   Creer  set mesh attach to!
	void Render(float fTimeElapsed);
	void Start(void);
	bool IsFinished(void);

	cTV_3DVECTOR Position;
	cTV_3DVECTOR Offset;

	CTVMesh* MeshAttachedTo;

private:
	CTVParticleSystem* pPartSys;
	CTVScene* pTVScene;

	float fDuration;
	float fTimerCount;
	bool bFinished;

	int i;
	int iEmitterCount;
	

};
