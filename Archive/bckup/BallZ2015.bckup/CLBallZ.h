#ifndef __BALLZ_H__
#define __BALLZ_H__
#pragma once
#include "TV3DMoteur.h"
#include "3DObject.h"  
#include "Sound.h"
#include "Speedo.h"

static bool bTest;  // isJumping, etc... pour enlever ces variable de 3DObject... accessible par tout ce qui include BallZ


class CLBallZ
{
public:
	CLBallZ(NewtonWorld* World, cTV_3DVECTOR pos, float fRadius, float fMasse, int iNewtonMat);
	~CLBallZ(void);
	void Update(void);
	void SetPosition(cTV_3DVECTOR Pos, cTV_3DVECTOR Dir);

	CL3DObject * clCtrl;
	CL3DObject * clShell;
	NewtonCollision * Collision;  //Shell
	
	cTV_3DVECTOR vTorque;
	cTV_3DVECTOR vCTRLTorque;
	float fThrottleStep;
	float fThrottle;
	float fMaxThrottle;

	CLSpeedo* clSpeedo;

private:
	CLTV3DMoteur* clTV3D;
	CLGlobalVar* clGlobalVar;
	float fRadius;
	float fMass;	

};
#endif
