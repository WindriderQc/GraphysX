#include "stdafx.h"
#include "KinematicPrimitive.h"

namespace GraphysX
{
	CLKinematicPrimitive::CLKinematicPrimitive(NewtonWorld* world, STEntityParam stParam) : CLPhysGeom(world, stParam)
	{
	}
	CLKinematicPrimitive::~CLKinematicPrimitive()
	{
	}

	void CLKinematicPrimitive::update()
	{
		//mesh->Enable(stParam.enable);
		//mesh->SetPosition(stParam.pos.x, stParam.pos.y, stParam.pos.z);
		//mesh->SetRotation(stParam.rot.x, stParam.rot.y, stParam.rot.z);
		//mesh->SetScale(stParam.scale.x, stParam.scale.y, stParam.scale.z);
	}

	void CLKinematicPrimitive::ForceNTorque()
	{
		//addGravity();    // A FAIRE: pour permettre l'interaction reel!!!!   Pos B - Pos A combiné avec Elapsed time

	}
	void CLKinematicPrimitive::Transform()
	{
		
		matrixTVmeshToBody();
		// NewtonBodySetVelocity(pbody, );    
		//"Note though, you will have to calculate the velocities yourself and set these to get correct collision response."
	}

	NewtonBody* CLKinematicPrimitive::loadBody()
	{
		NewtonBody* body = NewtonCreateKinematicBody(m_World, loadCollision(), &matrix_TVMeshToNewton()[0][0]);
	//	NewtonBodySetCollidable(body, 1);
	//	setMass(1.0f);

		return body;
	}
}