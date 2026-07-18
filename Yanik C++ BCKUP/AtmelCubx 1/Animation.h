#pragma once
#include "StdAfx.h"

#define MAXANIMS 16
#define ROTATE 666
#define TRANSLATE 667
#define CHANGECOLOR 668



class CLAnimation
{
public:

	CLAnimation(int iDuration, int iTypeAnim, bool bEtat);
	~CLAnimation(void);

	void Animate(CTVMesh* TV3DMesh, int iIntensity, cTV_3DVECTOR Force, cTV_3DVECTOR VecPosition, cTV_3DVECTOR VecRotation, float SpeedFactor);
	
protected:
		
	int iActualFrame;  // Compteur
	int iAnimNbrFrame;   // Durée
	bool bState; // active ou pas
	int iType;  //  Rotation, Translation, ColorChange, etc...
	int iIntensity;  

};