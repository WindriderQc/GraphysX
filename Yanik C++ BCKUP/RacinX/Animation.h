#pragma once
#include "StdAfx.h"



class CLAnimation
{
public:

	CLAnimation(int iDuration, bool bEtat);
	~CLAnimation(void);

	void Translate   (CTVMesh* TV3DMesh, cTV_3DVECTOR Force, float SpeedFactor);
	void Rotate      (CTVMesh* TV3DMesh, cTV_3DVECTOR Force, float SpeedFactor);
	void ChangeColor (CTVMesh* TV3DMesh, int Intensity, float SpeedFactor);
	void Stop(void);
	void Start(void);
	
protected:
		
	int iActualFrame;  // Compteur
	int iAnimNbrFrame; // Durée
	bool bState;	   // active ou pas 
	cTV_3DVECTOR VecRotation, VecPosition;

	CTVScene* pTVScene;
	

};


class CLAnimator
{

public:
	CLAnimator(void);
	~CLAnimator(void);

	void UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land, float fTimeElapsed);

protected:
	CTVScene* pTVScene;
	CTVMathLibrary* pMath;
};