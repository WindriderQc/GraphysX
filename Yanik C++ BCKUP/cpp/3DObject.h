#pragma once
#include "StdAfx.h"


//  Improver pour pouvoir creer des primitive gerer par la physic et lighting de base...  ( box, sphere, cylinder)
class CL3DObject
{
public:
	CL3DObject(char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float fMasse);
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
