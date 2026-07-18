#pragma once
#include "StdAfx.h"

class CLElevator
{
public:
	CLElevator(void);
	~CLElevator(void);

	void Render(void);


	CTVPath* path;
	cTV_3DVECTOR coord[4];

	CTVMesh* Mesh;

	CTVScene* pTVScene;
	CTVScreen2DImmediate* pTVScreen;
};
