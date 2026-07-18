#pragma once
#include "DynamicMesh.h"
#include "CustomSlidingContact.h"

namespace GraphysX
{
	class PushPlate : public CLDynamicMesh
	{
	public:
		GRAPHYSX_API PushPlate(NewtonWorld* world, STMeshParam param, dVector forcepush = { 0, 0, 100.0f }, dVector forcepull = { 0, 0, 1.0f }, float masse = 1.0f);
		GRAPHYSX_API ~PushPlate();

		// void update() override;
		GRAPHYSX_API void ForceNTorque() override;
		GRAPHYSX_API void setLimits(float limitsMinDist = 1.0f, float limitsMaxDist = 1.0f, float MinAngularLimit = 0, float MaxAngularLimit = 0);  // TODO: def value to be revised

		bool bTrigger = false;
	private:
		CustomSlidingContact* slider;
		dVector forcePush, forcePull;
	};
}