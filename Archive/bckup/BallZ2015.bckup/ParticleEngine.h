#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#pragma once
#include "TV3DMoteur.h"

class CLExplosion
{
public:
	CLExplosion(void);
	~CLExplosion(void);


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
class CLEffect
{
public:
	CLEffect(char* PathToTVM, cTV_3DVECTOR position, cTV_3DVECTOR offset, CTVMesh* AttachedTo, int duration);
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
class CLParticleEngine
{
public:
	CLParticleEngine();
	~CLParticleEngine();
	void UpdateRender();

public:
	CLGlobalVar *clGlobalVar;
	CLExplosion *clExplosion;
	CLEffect *clEffect;

};


class CLBillboardAnim
{
public:

	CLBillboardAnim();
	~CLBillboardAnim();
	
};
#endif