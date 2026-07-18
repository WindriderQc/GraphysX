#pragma once
#include "3DObject.h"

#define ROTSPEED 2

class CLCubZ :
	public CL3DObject
{
public:
	CLCubZ(char* PathToMesh, float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable);
	~CLCubZ(void);

	void Animate(float ElapsedTime);
	void StartSaversAnimation(void);
	void RotateTo(int iCubeIndex);
	void BackRotate(void);
	void Open(void);
	void Close();
	void Disappear(void);
	void RenderActor(void);
	int AnimationCount;

	CTVMesh* pCubXBtn[8];
private:
	CTVActor* CubXSaversActor;
	CTVActor* CubeOpenActor;
	CTVActor* CubXRotationActor;

	bool bIsRotating;
	bool bIsBackRotating;
	bool bIsOpening;
	bool bIsClosing;
	bool bIsDisappearing;

	int iSelectedCube;
	float fSize;

};
