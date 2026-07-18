#include "StdAfx.h"
#include "Speedo.h"

CLSpeedo::CLSpeedo()
{
	pMath = new CTVMathLibrary();
	vOldPos = cTV_3DVECTOR(0, 0, 0);
	fSpeed = 0;
}
CLSpeedo::~CLSpeedo()
{
	if (pMath) { delete(pMath); pMath = NULL; }
}
void CLSpeedo::CalculateSpeed(cTV_3DVECTOR vActualPos, float fTimeElapsed)
{
	cTV_3DVECTOR vSpeed = vActualPos - vOldPos;
	fSpeed = (pMath->VLength(&vSpeed) / fTimeElapsed) * 1000;
	vOldPos = vActualPos;
}