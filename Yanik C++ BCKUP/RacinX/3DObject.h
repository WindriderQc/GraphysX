#pragma once
#include "StdAfx.h"

class CL3DObject
{
public:
	CL3DObject(char* PathToMesh, float Size, int Texture, cTV_3DVECTOR Pos, bool bEnable, float fMasse);
	~CL3DObject(void);

	void SetOpacity(float fOpacity);
	CTVMesh* GetMesh(void);
	int GetPhysicBody(void);
	cTV_3DVECTOR GetPosition(void);
	void SetPosition(cTV_3DVECTOR);
	void Render(void);
	
protected:
	CTVMesh* Mesh;
	int iStdMaterial;
	int iPhysicBody;
	float fMasse;
	CTVScene* pTVScene;
	CTVPhysics* pTVPhysics;
	CTVMaterialFactory* pMatFactory; 
	
};
