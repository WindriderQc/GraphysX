#pragma once
#include "StdAfx.h"

class CLSlideObject
{
public:
	CLSlideObject(char* PathToMesh, float Size, cTV_3DVECTOR Pos, bool bEnable);
	~CLSlideObject(void);

	CTVMesh* GetMesh(void);
	int GetPhysicBody(void);
	cTV_3DVECTOR GetPosition(void);
	void SetPosition(cTV_3DVECTOR);
	void Render(void);
	
protected:
	CTVMesh* Mesh;
	int iStdMaterial;
	int iPhysicBody;
	CTVScene* pTVScene;
	CTVPhysics* pTVPhysics;
	CTVMaterialFactory* pMatFactory; 
	
};
