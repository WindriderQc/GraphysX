#pragma once
#include "StdAfx.h"
#include <string>

class CLLand
{
public:
	CLLand(std::string HeightmapPath, std::string TexturePath, float fWidth, float fHeight, cTV_3DVECTOR Pos);
	~CLLand(void);

	void Render(void);
	int GetPhysicBody(void);
	void SetPos(cTV_3DVECTOR pos);

private:
	CTVLandscape* pTVLand;

	CTVPhysics* pTVPhysics;
	CTVScene* pTVScene;
	CTVTextureFactory* pTVTexFactory; 
	CTVMaterialFactory* pTVMaterialfactory;

	int matLand;
	int iLandPhysicBody;
	int texGround;

};
