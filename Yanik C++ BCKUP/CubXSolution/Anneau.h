#pragma once
#include "StdAfx.h"
#include "3DObject.h"
//#include "Animation.h"

class CLAnneau
{
public:
	CLAnneau(void);
	~CLAnneau(void);

private:
	//CTVMesh* OriginalRing;
	CL3DObject* OriginalRing;

	struct STRing
	{
	CL3DObject* clRingMesh;
	//CTVMesh* Mesh;
	int iColor;
	float fSize;
	int iPtsValue;
	}stRings[100];
	

	//CLAnimation clAnimRing;
	CTVScene* pTVScene;

public:
	int GetPtsValue(int RingIndex);
	void SetPtsValue(int value, int RingIndex);
	cTV_3DVECTOR GetRingPos(int RingIndex);
	void SetRingPos(cTV_3DVECTOR pos, int RingIndex);
	float GetRingSize(int RingIndex);
	void SetRingSize(float size, int RingIndex);
	CTVMesh* GetRingMesh(int RingIndex);	
	
	void Render(void);
};