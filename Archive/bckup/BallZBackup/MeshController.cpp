#include "stdafx.h"
#include "MeshController.h"

void PistonForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
{ // Force and Torque callback (called everytime a modification occurs on a body)

	float masse;
	dVector inertie;
	dVector force;
	dVector Pulse;

	// On récupère en premier lieu la masse ainsi que l'inertie
	NewtonBodyGetMassMatrix(body, &masse, &inertie.m_x, &inertie.m_y, &inertie.m_z);
	force.m_x = 0.0f;
	force.m_y = -masse * GRAVITY;
	force.m_z = 0.0f;
	//NewtonBodyAddForce(body, &force.m_x); // On ajoute la force au corps
	
	MeshController* Ctrl = NULL;
	Ctrl = (MeshController*)NewtonBodyGetUserData(body);
	if (Ctrl)
	{
		if (Ctrl->bIsTriggered)
		{
			Pulse.m_x = 0;
			Pulse.m_y = 0;
			Pulse.m_z = -100;
			NewtonBodySetForce(body, &Pulse.m_x); // On ajoute la force au corps
			//CLSound::getInstance()->PlaySound(3);
		}
		else
		{
			Pulse.m_x = 0;
			Pulse.m_y = 0;
			Pulse.m_z = 1;
			NewtonBodySetForce(body, &Pulse.m_x); // On ajoute la force au corps
			//CLSound::getInstance()->PlaySound(3);
		}
	}
	else
		std::cout << "MeshController not found";
}
void BallZForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
{
	
	float masse;
	cTV_3DVECTOR inertie;
	cTV_3DVECTOR force;
	dVector Torque, Jump;
	// On récupère en premier lieu la masse ainsi que l'inertie
	NewtonBodyGetMassMatrix(body, &masse, &inertie.x, &inertie.y, &inertie.z);
	force.x = 0.0f;
	force.y = -masse * GRAVITY;
	force.z = 0.0f;
	NewtonBodyAddForce(body, &force.x); 
	
	//fThrottleStep = 1;
	MeshController* Ctrl = NULL;
	Ctrl = (MeshController*)NewtonBodyGetUserData(body);
	if (Ctrl)
	{
		if (Ctrl->bJumping)
		{
			if (Ctrl->bAllowJumps)   
			{
				
				Jump.m_x = 0;
				Jump.m_y = 500;
				Jump.m_z = 0;
				NewtonBodyAddForce(body, &Jump.m_x); // On ajoute la force au corps
				//CLSound::getInstance()->PlaySound(3);
				
			}
			Ctrl->bJumping = false;
			Ctrl->bAllowJumps = false;
		}
		bool bCTRLBALL = true;
		if (bCTRLBALL)
		{
			dVector Omega;
			Torque.m_x = Ctrl->vCtrlTorque.x;
			Torque.m_y = Ctrl->vCtrlTorque.y;
			Torque.m_z = Ctrl->vCtrlTorque.z;
			NewtonBodyGetOmega(body, &Omega.m_x);
			dVector finaltorque(Torque.Scale(inertie.x *75.0f) - Torque.Scale(2.0f * inertie.x * (Omega % Torque)));
			NewtonBodyAddTorque(body, &Torque.m_x);
		}
	
		if (Ctrl->bIsTriggered)
	{
		//find a way to kick ass with piston
		int i = 0;
	}
	}


}

void ControlledTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{ // transformation callback (called after the body has been simulated)
	cTV_3DMATRIX MX;
	NewtonBodyGetMatrix(body, MX.m[0]);
	((MeshController*)NewtonBodyGetUserData(body))->Obj->Mesh->SetMatrix(&MX);
	//char* Name = ((CTVMesh*)NewtonBodyGetUserData(body))->GetMeshName();
}
MeshController::MeshController(CLMesh* ControlledMeshName, bool bEnable, CLMeshFactory* meshFactory)
{	
	CTVMathLibrary pTVMaths;
	MeshFactory = meshFactory;
	bIsTriggered = false;
	bIsFinishline = false;

	vTorque = cTV_3DVECTOR(0, 0, 0);
	fThrottleStep = 0.05f;
	fThrottle = 0.0f;
	fMaxThrottle = 0.5f;

	fTime = 0;
	Obj = ControlledMeshName;
	if (!strcmp(Obj->Mesh->GetMeshName(), "BallZ"))
	{
		NewtonBodySetForceAndTorqueCallback(Obj->clPhysicEntity->pbody, BallZForceAndTorqueCallback);
	}
	else{
		NewtonBodySetForceAndTorqueCallback(Obj->clPhysicEntity->pbody, PistonForceAndTorqueCallback);
		std::cout << "Pistonctrler";
	}
	
	NewtonBodySetTransformCallback(Obj->clPhysicEntity->pbody, ControlledTransformationCallback);
	
	std::cout << " Controller constructed  with: " << Obj->Mesh->GetMeshName() << std::endl;
}
MeshController::~MeshController(void)
{
	if(pTVMaths) delete(pTVMaths);
}

void MeshController::Update(cTV_3DVECTOR Mouse3DPos)
{
	
		// CALCULATE TORQUE from Object to MousePosition
		cTV_3DVECTOR vNewtonTorque;
		Position = Obj->Mesh->GetPosition();
		MouseDirection = Mouse3DPos - Position;
		if (bThrottle)
		{
			vTorque.x = MouseDirection.x;
			vTorque.y = 0;
			vTorque.z = MouseDirection.z;
			// Valider si Y reste tjrs a 0 apres la normalisation... sinon ajuster directement la valeur recu du gamepad.
			vTorque = pTVMaths->VNormalize(&vTorque);
			// Tweak pour setter le torque dans le bon sens... car Newton et TV3D ne semble pas avoir le meme systeme d'Axe
			vNewtonTorque.x = vTorque.z;
			vNewtonTorque.y = vTorque.y;
			vNewtonTorque.z = -(vTorque.x);

			fThrottle = fThrottle + fThrottleStep;
			if (fThrottle > fMaxThrottle)  fThrottle = fMaxThrottle;
		}
		else {
			fThrottle = fThrottle - fThrottleStep;
			if (fThrottle < 0)  fThrottle = 0;
			}

		vCtrlTorque = vNewtonTorque * fThrottle;
	
}

