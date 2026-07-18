#include "Animation.h"


// Constructeur
CLAnimation::CLAnimation(int iDuration, int iTypeAnim, bool bEtat)
{
iAnimNbrFrame = iDuration;
iType = iTypeAnim;
bState = bEtat;

iActualFrame = 0;
iIntensity = 0;
}

//Destructeur
CLAnimation::~CLAnimation(void)
{

}

void CLAnimation::Animate(CTVMesh* TV3DMesh, int iIntensity, cTV_3DVECTOR Force, cTV_3DVECTOR VecPosition, cTV_3DVECTOR VecRotation, float SpeedFactor)
{

if(bState == 1)
{
	if((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
	{
	switch(iType)
	{
	case ROTATE:
		VecRotation.x = VecRotation.x + (Force.x * SpeedFactor);// Force est la rotation en degré pour chaque axes
		if(VecRotation.x >360) VecRotation.x = 0;
		VecRotation.y = VecRotation.y + (Force.y * SpeedFactor); 
		if(VecRotation.y >360) VecRotation.y = 0;
		VecRotation.z =	VecRotation.z + (Force.z * SpeedFactor);
		if(VecRotation.z >360) VecRotation.z = 0;
		TV3DMesh->SetRotation(VecRotation.x, VecRotation.y, VecRotation.z);  
		break;
	case TRANSLATE:
		TV3DMesh->SetPosition(VecPosition.x + (Force.x * SpeedFactor), VecPosition.y + (Force.y * SpeedFactor), VecPosition.z + (Force.z * SpeedFactor));
		break;
	case CHANGECOLOR:
		break;
	default:
		break;
	}
	}
iActualFrame++;
}

}