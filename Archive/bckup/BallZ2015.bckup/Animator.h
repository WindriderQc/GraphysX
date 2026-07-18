#pragma once
#include "MeshFactory.h"

class CLAnimator
{
public:
	CLAnimator(){
		iAnimNbrFrame = -1; // Durée de l'animation
		bEtat = true;  // Animation Start/Stop
		iActualFrame = 0;  // Compteur
		bAnimState = true;	   // active ou pas 
		// fTime = NULL;
	};

	//Animation
	void Translate(CLMesh* Obj, cTV_3DVECTOR Force, float SpeedFactor)
	{
		cTV_3DVECTOR VecRotation = Obj->Mesh->GetRotation();
		cTV_3DVECTOR VecPosition = Obj->Mesh->GetPosition();
		if (bAnimState == 1)
		{
			if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini ou pas terminée
			{
				VecPosition.x = VecPosition.x + (Force.x * SpeedFactor);
				VecPosition.y = VecPosition.y + (Force.y * SpeedFactor);
				VecPosition.z = VecPosition.z + (Force.z * SpeedFactor);

				Obj->SetPosNRot(VecPosition, VecRotation);
				iActualFrame++;
			}
			else {
				bAnimState = 0;
			}
		}
	}
	void Rotate(CLMesh* Obj, cTV_3DVECTOR Force, float SpeedFactor)
	{
		cTV_3DVECTOR VecRotation = Obj->Mesh->GetRotation();
		cTV_3DVECTOR VecPosition = Obj->Mesh->GetPosition();

		if (bAnimState == 1)
		{
			if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
				VecRotation.x = VecRotation.x + (Force.x * SpeedFactor);// Force est la rotation en degré pour chaque axes
				VecRotation.y = VecRotation.y + (Force.y * SpeedFactor);
				VecRotation.z = VecRotation.z + (Force.z * SpeedFactor);
				if (VecRotation.x >= 360)  VecRotation.x = 0;
				if (VecRotation.y >= 360)  VecRotation.y = 0;
				if (VecRotation.z >= 360)  VecRotation.z = 0;
				
				//Obj->Mesh->RotateX(Force.x * SpeedFactor, true);
				//Obj->Mesh->RotateY(Force.y * SpeedFactor, true);
				//Obj->Mesh->RotateZ(Force.z * SpeedFactor, true);
				Obj->SetPosNRot(VecPosition, VecRotation);
				iActualFrame++;
			}
			else {
				bAnimState = 0;
			}
		}
	}/*
	void CL3DObject::RotateAround(float SpeedFactor, cTV_3DVECTOR *Center, float fRadius)
	{
		float xRot, yRot;
		cTV_3DVECTOR rotate;
		cTV_3DVECTOR VecRotation = Mesh->GetRotation();
		cTV_3DVECTOR VecPosition = Mesh->GetPosition();


		xRot = (float)(2.0f * M_PI * fTime - M_PI * 0.5f);
		yRot = 0;

		fTime = fTime + SpeedFactor;

		if (bAnimState == 1)
		{
			if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
				rotate = clTV3D->pTVMaths->MoveAroundPoint(Center, fRadius, xRot, yRot);
				SetPosNRot(rotate, VecRotation);
				iActualFrame++;
			}
			else {
				bAnimState = 0;
			}
		}
	}
	void CL3DObject::ChangeColor(int Intensity, float SpeedFactor)
	{
		if (bAnimState == 1)
		{
			if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
				// A Implenter....
				iActualFrame++;
			}
			else {
				bAnimState = 0;
			}
		}
	}
	*/

	int iAnimNbrFrame; // Durée de l'animation
	bool bEtat;  // Animation Start/Stop
	//void Translate(cTV_3DVECTOR Force, float SpeedFactor);
	//void Rotate(cTV_3DVECTOR Force, float SpeedFactor);
	//void RotateAround(float SpeedFactor, cTV_3DVECTOR *Center, float fRadius);
	//void ChangeColor(int Intensity, float SpeedFactor);
	void Stop(void) { bAnimState = false; }
	void Start(void) { bAnimState = true; }
	void Reset(void) { iActualFrame = 0; }
	void ChangeAnimDuration(int iFrameNbr) { iAnimNbrFrame = iFrameNbr; }
private:
	int iActualFrame;  // Compteur
	bool bAnimState;	   // active ou pas 
	//float fTime;
};

