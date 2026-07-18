#include "stdafx.h"
#include "PushPlate.h"

namespace GraphysX
{
	PushPlate::PushPlate(NewtonWorld* world, STEntityParam param, dVector forcepush, dVector forcepull) 
		: CLDynamicMesh(world, param)
	{
		forcePush = forcepush;
		forcePull = forcepull;
		// connect the bodies by a Slider joint
		dMatrix matrix;
		NewtonBodyGetMatrix(pbody, &matrix[0][0]);
		slider = new CustomSlidingContact(matrix, pbody, nullptr);
		//setLimits(); // sets default limits 
	}
	
	PushPlate::~PushPlate()
	{
		if(slider) delete(slider); slider = nullptr;
	}

	void PushPlate::setLimits(float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit)  
	{
		slider->EnableLinearLimits(true);
		slider->SetLinearLimis(limitsMinDist, limitsMaxDist);
		// enable limit of first axis
		slider->EnableAngularLimits(true);
		slider->SetAngularLimis(MinAngularLimit, MaxAngularLimit);
	}

	void PushPlate::ForceNTorque()
	{ 
		addGravity();

		dVector pulse;
		if (bTrigger) {
			NewtonBodySetForce(pbody, &forcePush.m_x);
			bTrigger = false;
		}
		else {
			NewtonBodySetForce(pbody, &forcePull.m_x);
		}
	}
}