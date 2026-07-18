#pragma once
#include "StdAfx.h"
#include "3DObject.h"

class CLBallZ
{
public:
	CLBallZ(void);
	~CLBallZ(void);


private:
	CL3DObject* clBallZShell;
	CL3DObject* clBallZCtrl;

public:
	CTVMesh* GetBallZMesh();
	cTV_3DVECTOR GetBallZPos();
	void SetBallZPos(cTV_3DVECTOR pos);
	
	
	void Render(void);
};
