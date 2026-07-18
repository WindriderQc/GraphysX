#pragma once
#include "StdAfx.h"

class CLLand
{
public:
	CLLand(string HeightmapPath, string TexturePath, float fWidth, float fHeight, cTV_3DVECTOR Pos);
	~CLLand(void);

	virtual void Render(void);
	virtual void SetPos(cTV_3DVECTOR pos);
	int GetPhysicBody(void);

protected:
	CTVLandscape* pTVLand;

	CTVPhysics* pTVPhysics;
	CTVScene* pTVScene;
	CTVTextureFactory* pTVTexFactory; 
	CTVMaterialFactory* pTVMaterialfactory;

	int matLand;
	int iLandPhysicBody;
	int texGround;
};
