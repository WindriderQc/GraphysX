#pragma once

#include "StdAfx.h"


class CLEffect
{
public:
	CLEffect(char* PathToTVM, cTV_3DVECTOR position, cTV_3DVECTOR offset,  CTVMesh* AttachedTo, int duration );
	~CLEffect(void);

	void Render(float fTimeElapsed);
	void Start(void);
	bool IsFinished(void);
	void SetMeshAttachedTo(CTVMesh* mesh);

	cTV_3DVECTOR Position;
	cTV_3DVECTOR Offset;

	CTVMesh* MeshAttachedTo;
	bool bSystemStarted;

private:
	CTVParticleSystem* pPartSys;
	CTVScene* pTVScene;

	float fDuration;
	float fTimerCount;
	bool bFinished;

	int i;
	int iEmitterCount;
	

};
