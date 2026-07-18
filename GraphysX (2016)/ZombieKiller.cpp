#include "stdafx.h"
#include "ZombieKiller.h"
#include "ResourceManager.h"


namespace GraphysX
{
	ZombieKiller::ZombieKiller(CLMeshPrimitive* cage, CLMeshPrimitive* insidePrim, NewtonWorld* world, STMeshParam param, float masse)
		: Human(world, param, masse), outsideBall(cage)
	{
		setPhysmat(ePHYSMAT::BALL);
		insideBall = insidePrim;
		//insideBall->mesh->RotateX(-90);    //  devrait pas etre nécessaire puisque le -90 est passé dans STMeshPara a la creation 
				
		mesh->Destroy();
		mesh = outsideBall->mesh;
		
		NewtonBodySetUserData(pbody, this);

		fThrottleStep = 0.05f;
		fThrottle = 0.0f;
		fMaxThrottle = 0.5f;
	}
	ZombieKiller::~ZombieKiller()
	{
	}

	void ZombieKiller::update()
	{
		if (bThrottle)
		{
			fThrottle = fThrottle + fThrottleStep;
			if (fThrottle > fMaxThrottle)  fThrottle = fMaxThrottle;
		}
		else {
			fThrottle = fThrottle - fThrottleStep;
			if (fThrottle < 0)  fThrottle = 0;
		}
		// CALCULATE TORQUE from Object to MousePosition
		cTV_3DVECTOR Ball3DPos = mesh->GetPosition();
		MouseDirection = mouse3Dpos - Ball3DPos;   //   TODO: PAS CHIC....   Mouse3Dpos doit etre updaté somewhere pour que ca marche
		cTV_3DVECTOR MouseTorque;
		MouseTorque.x = MouseDirection.x;
		MouseTorque.y = 0;
		MouseTorque.z = MouseDirection.z;
		MouseTorque = GraphysX::CLTV3D::pTVMaths.VNormalize(&MouseTorque);
		// Tweak pour setter le torque dans le bon sens... car Newton et TV3D ne semble pas avoir le meme systeme d'Axe
		dVector vNewtonTorque;
		vNewtonTorque.m_x = MouseTorque.z;
		vNewtonTorque.m_y = MouseTorque.y;
		vNewtonTorque.m_z = -(MouseTorque.x);
		TorqueToApply = vNewtonTorque.Scale(fThrottle);

		BallDirectionTag.Start = Ball3DPos;
		cTV_3DVECTOR orientation;
		math.TVVec3Normalize(&orientation, &MouseDirection);
		//BallDirectionTag.End = Tools3D->m_MeshFactory->BallZCtrler->Position + orientation;
		BallDirectionTag.End = Ball3DPos + ((orientation * fThrottle) * 2);
		BallDirectionTag.Color = RGBA_GREEN;
		



		GraphysX::CLSound::updateListener(Ball3DPos, getVelocity(), orientation, Vector3(0, 0, 0));
	}

	CLMeshPrimitive* ZombieKiller::getInsideMesh()
	{
		return(insideBall);
	}
	void ZombieKiller::ForceNTorque()
	{
 		addGravity();

		if (bJumping)
		{
			if (bAllowJumps)
			{
				dVector Jump(0,275,0);
 				NewtonBodyAddForce(pbody, &Jump.m_x); // On ajoute la force au corps
				//CLSound::getInstance()->PlaySound(3);
				bAllowJumps = false;
			}
			bJumping = false;
		}
		bool bCTRLBALL = true;
		if (bCTRLBALL)
		{
			dVector Omega;
			NewtonBodyGetOmega(pbody, &Omega.m_x);
			cTV_3DVECTOR inertie;
			float tmp;
			// On récupère en premier lieu la masse ainsi que l'inertie
			NewtonBodyGetMassMatrix(pbody, &tmp, &inertie.x, &inertie.y, &inertie.z);
			dVector finaltorque(TorqueToApply.Scale(inertie.x *75.0f) - TorqueToApply.Scale(2.0f * inertie.x * (Omega % TorqueToApply)));
			NewtonBodyAddTorque(pbody, &TorqueToApply.m_x);
		}
		/*if (bIsTriggered)
		{
		//find a way to kick ass with piston
		int i = 0;
		}*/
	}
	void ZombieKiller::Transform()
	{
		cTV_3DMATRIX* mat = matrix_bodyToTV();
		mesh->SetMatrix(mat);
		cTV_3DVECTOR Ball3DPos(0, 0, 0);
		Ball3DPos = mesh->GetPosition();
		insideBall->mesh->SetPosition(Ball3DPos.x, Ball3DPos.y, Ball3DPos.z);
		insideBall->mesh->LookAtPoint(&mouse3Dpos, true);

		/*float BallX = 0.0f;
		float BallY = 0.0f;
		math.Project3DPointTo2D(&Ball3DPos, &BallX, &BallY, true);
		//std::cout << "  . projected" << std::endl;
		float posX = BallX - GraphysX::CLTV3D::stMouse.PosX;
		float posY = BallY - GraphysX::CLTV3D::stMouse.PosY;
		float angle = math.Direction2Ang(posX, posY);
		//insideBall->mesh->SetRotation(Ball3DRot.x-90, -angle, Ball3DRot.z); // 90    aligner la texture
		insideBall->mesh->SetRotation(-90, -angle, 0); // 90    aligner la texture    //  TODO: check pkoi cest affecté par la pos de la cam...   l'angle requis change si on chage la pos de la cam.
		*/
	}
}