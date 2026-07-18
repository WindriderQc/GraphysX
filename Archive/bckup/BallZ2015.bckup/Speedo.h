#ifndef __SPEEDO_H__
#define __SPEEDO_H__
#pragma once
#include "TV3DMoteur.h" 

class CLSpeedo
{
public:
	CLSpeedo(cTV_3DVECTOR StartPos);
	~CLSpeedo();
	float CalculateSpeed(cTV_3DVECTOR vActualPos, float fTimeElapsed);

private:
	cTV_3DVECTOR vOldPos;
	CTVMathLibrary* pMath;
	float fSpeed;
};
#endif