#pragma once
#include "StdAfx.h"
#include "3DObject.h"

class CLBall
{
public:
	CLBall(void);
	~CLBall(void);

	void PushBall(int iDir);
	void SetSpeed(int speed);
	void SetDirection(cTV_3DVECTOR);
	void SetPosition(cTV_3DVECTOR);

	cTV_3DVECTOR GetSpeed(void);
	cTV_3DVECTOR GetDirection(void);
	cTV_3DVECTOR GetPosition(void);

	void Render(void);

	CTVMesh* GetMesh(void);
	int GetPhysicBody(void);
		
private:
	int iSpeed, iAcceleration;
	
	float fMasse;
	cTV_3DVECTOR Position;
	cTV_3DVECTOR Direction;
	cTV_3DVECTOR Torque, TorqueFront, TorqueBack, TorqueLeft, TorqueRight;

	int RotFront, RotLeft, RotRight, RotBack;  // for testing purpose

	CL3DObject* clBallObject;
	CTVPhysics* pTVPhysics;
	//int iTexture;
	//int iMaterial;

	int iFriction;
	int iRebondissement;

	float fSize;
		
//  a implementer  
//	CLPartSys clSpecialEffects[10];
//	enum{Explode, glow, FireBall,WindTrail,Disapear,5,6,7,8,9}
	void LoadEffects(void);
};
