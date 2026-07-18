#pragma once
#include "DynamicMesh.h"




namespace GraphysX
{
	class CLMeshBillboard;
	
	class Human : public CLDynamicMesh
	{
	public:
		GRAPHYSX_API Human(NewtonWorld* world, STMeshParam param, float masse = 1.0f, int tex = NULL);
		GRAPHYSX_API virtual ~Human();

		GRAPHYSX_API void update() override;
		GRAPHYSX_API void ForceNTorque() override;

		GRAPHYSX_API void setRandomDirection();
		GRAPHYSX_API void rotateDirection();

	private:

		cTV_3DVECTOR direction;
		int frameCount = 0;
		CLMeshBillboard* billboard = nullptr;
	};

}