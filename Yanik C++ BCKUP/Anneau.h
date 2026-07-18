#pragma once
#include "StdAfx.h"
//#include "Animation.h"

class CLAnneau
{
public:
	CLAnneau(void);
	~CLAnneau(void);

private:
	CTVMesh* OriginalRing;
	
	struct STRing
	{
	CTVMesh* Mesh;
	int iColor;
	int iSize;
	int iPtsValue;
	cTV_3DVECTOR Pos;
	}stRings[100];
	

	//CLAnimation clAnimRing;
	CTVScene* pTVScene;

public:
	int GetPtsValue(int RingIndex);
	void SetPtsValue(int value, int RingIndex);
	cTV_3DVECTOR GetRingPos(int RingIndex);
	void SetRingPos(cTV_3DVECTOR pos, int RingIndex);
	int GetRingSize(int RingIndex);
	void SetRingSize(int size, int RingIndex);
	CTVMesh* GetRingMesh(int RingIndex);	
	
	void Render(void);
};