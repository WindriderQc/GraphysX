#pragma once
#include "3DObject.h"
#include "MenuManager.h"
#include "Animation.h"

#define ROTSPEED 2

#define SLEEPING		1
#define POSITIONNING	2
#define FOCUSING		3
#define SELECTING		4
#define OPENING			5
#define CLOSING			6
#define DESELECTING		7
#define FOCUSED			8
#define OPENED			9
#define DEFOCUSING		10

class CLCubX
{
public:
	CLCubX(float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable);
public:
	~CLCubX(void);

	void StartSaversAnimation(void);
	void FadeOff(void);
	void FadeOn(void);
	void AnimateCubX(float ElapsedTime);
	void SetCubXMenuButtons(void);
	void DisableCubXButtons(void);
	void EnableCubXButtons(void);
	void Render(void);
	CTVMesh* GetMesh(void);

	int iActualState;
	int iSelectedCube;
	CLMenuManager* clCubXMenuMgr;
	CTVMesh* CubXMesh;
	CTVMesh* pCubXBtn[8];
	CTVMesh* MainExitBtn;

private:
		CLAnimation* clRotationAnim;    
		cTV_3DVECTOR RotForce, SpeedyRotForce;

		cTV_3DVECTOR InitialRotVec;
		CTVCamera* pTVCamera;
		cTV_3DVECTOR CamSpeed;
		CTVActor* CubXRotActor[8];
		CTVActor* CubXOpenActor;

		int iStdMaterial;
		int iPhysicBody;
		float fMasse;
		CTVScene* pTVScene;
		CTVPhysics* pTVPhysics;
		CTVMaterialFactory* pMatFactory; 
		
};
