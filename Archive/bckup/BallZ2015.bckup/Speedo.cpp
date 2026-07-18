#include "StdAfx.h"
#include "Speedo.h"

CLSpeedo::CLSpeedo(cTV_3DVECTOR StartPos)
{
	pMath = new CTVMathLibrary();
	vOldPos = StartPos;
	fSpeed = 0.0f;
}
CLSpeedo::~CLSpeedo()
{
	if (pMath) { delete(pMath); pMath = NULL; }
}
float CLSpeedo::CalculateSpeed(cTV_3DVECTOR vActualPos, float fTimeElapsed)
{
	cTV_3DVECTOR vSpeed = vActualPos - vOldPos;
	fSpeed = (pMath->VLength(&vSpeed) / fTimeElapsed) * 1000;
	vOldPos = vActualPos;
	return(fSpeed);
}