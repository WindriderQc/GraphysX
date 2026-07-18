#pragma once
#include "Mesh.h"
#include "Speedo.h"

class CLMeshFactory;


class MeshController
{
public:
	MeshController(CLMesh* ControlledMeshName, bool bEnable, CLMeshFactory* meshFactory);
	~MeshController(void);

	void NewtonDebug(bool Body, bool Velocity)
	{
		/*if (pbody != NULL)
		{
		dMatrix matrix;
		NewtonBodyGetMatrix(pbody, &matrix[0][0]);

		if (Body){
		NewtonCollisionForEachPolygonDo(NewtonBodyGetCollision(pbody), &matrix[0][0], DebugShowGeoCollision, NULL);
		}
		if (Velocity){
		cTV_3DVECTOR Velo;
		NewtonBodyGetVelocity(pbody, &Velo.x);
		clTV3D.DrawTV3DLine(cTV_3DVECTOR(matrix.m_posit.m_x, matrix.m_posit.m_y, matrix.m_posit.m_z),
		cTV_3DVECTOR(matrix.m_posit.m_x + Velo.x, matrix.m_posit.m_y + Velo.y, matrix.m_posit.m_z + Velo.z), GREEN_RGBA);
		}
		}*/
	};
	
	CLSpeedo clSpeedo;
	
	CLMesh* Obj;
	// Info pour échange avec physics engine
	cTV_3DVECTOR vCtrlTorque, vTorque;
	float fThrottleStep;
	float fThrottle;
	float fMaxThrottle;

	cTV_3DVECTOR MouseDirection;
	cTV_3DVECTOR Position;

	bool bThrottle;
	bool bCTRLBALL;
	bool bJumping;
	bool bAllowJumps; 
	bool bIsFinishline; 
	bool bIsTriggered; 
private:
	CLMeshFactory* MeshFactory;

	// Animation 
public:
	int iAnimNbrFrame; // Durée de l'animation
	bool bEtat;  // Animation Start/Stop
	void Translate(cTV_3DVECTOR Force, float SpeedFactor)
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
		else{
			bAnimState = 0;
			}
		}
	};
	void Rotate(cTV_3DVECTOR Force, float SpeedFactor){
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
			if (VecRotation.z >= 360) 	VecRotation.z = 0;

			Obj->SetPosNRot(VecPosition, VecRotation);
			iActualFrame++;
			}
		else{
			bAnimState = 0;
			}
		}
	};
	void RotateAround(float SpeedFactor, cTV_3DVECTOR *Center, float fRadius){
		float xRot, yRot;
		cTV_3DVECTOR rotate;
		cTV_3DVECTOR VecRotation = Obj->Mesh->GetRotation();
		cTV_3DVECTOR VecPosition = Obj->Mesh->GetPosition();


		xRot = (float)(2.0f * PI * fTime - PI * 0.5f);
		yRot = 0;

		fTime = fTime + SpeedFactor;

		if (bAnimState == 1)
		{
			if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
				rotate = GraphysX::CLTV3D::pTVMaths.MoveAroundPoint(Center, fRadius, xRot, yRot); 
				//rotate = GraphysX::CLTV3D::getpTVMaths().MoveAroundPoint(Center, fRadius, xRot, yRot);
			Obj->SetPosNRot(rotate, VecRotation);
			iActualFrame++;
			}
		else{
			bAnimState = 0;
			}
		}
	};
	void ChangeColor(int Intensity, float SpeedFactor){
		if (bAnimState == 1)
		{
			if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
			// A Implenter....
			iActualFrame++;
			}
		else{
			bAnimState = 0;
			}
		}
	};
	void Stop(void) { bAnimState = false; }
	void Start(void){ bAnimState = true; }
	void Reset(void){ iActualFrame = 0; }
	void ChangeAnimDuration(int iFrameNbr){ iAnimNbrFrame = iFrameNbr; }

private:
	int iActualFrame;  // Compteur
	bool bAnimState;	   // active ou pas 
	float fTime;
	CTVMathLibrary* pTVMaths;

public:
	GRAPHYSX_API void Update(cTV_3DVECTOR Mouse3DPos);
};