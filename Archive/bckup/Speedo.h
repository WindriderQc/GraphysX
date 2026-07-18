#ifndef __SPEEDO_H__
#define __SPEEDO_H__
#pragma once
#include "TV3DMoteur.h" 

class CLSpeedo
{
public:
	CLSpeedo();
	~CLSpeedo();
	void CalculateSpeed(cTV_3DVECTOR vActualPos, float fTimeElapsed);

	float fSpeed;

private:
	cTV_3DVECTOR vOldPos;
	CTVMathLibrary* pMath;
};
#endif