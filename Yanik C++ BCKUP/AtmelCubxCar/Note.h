#pragma once
#include "StdAfx.h"

class CLNote
{
public:
	CLNote(CTVMesh* OriginalNoteMesh);
	~CLNote(void);

	void SetPos(void);


	CTVMesh* pNoteMesh;
	cTV_3DVECTOR Pos;

private:
	CTVScene* pTVScene;

};
