#pragma once
#include "StdAfx.h"

class CLBall
{
public:
	CLBall(void);
	~CLBall(void);

	void SetSpeed(int speed);
	void SetDirection(cTV_3DVECTOR);
	void SetPosition(cTV_3DVECTOR);

	int GetSpeed(void);
	cTV_3DVECTOR GetDirection(void);
	cTV_3DVECTOR GetPosition(void);

	void Render(void);

	CTVMesh* GetMesh(void);
		
private:
	int iSpeed, iAcceleration;
	cTV_3DVECTOR Position;
	cTV_3DVECTOR Direction;

	CTVMesh* BallMesh;
	//int iTexture;
	//int iMaterial;

	int iFriction;
	int iRebondissement;
	
	CTVScene* pTVScene;
	//CTVTextureFactory* pTVTexFac;
	
	//  a implementer  
//	CLPartSys clSpecialEffects[10];
//	enum{Explode, glow, FireBall,WindTrail,Disapear,5,6,7,8,9}
	void LoadEffects(void);
};
