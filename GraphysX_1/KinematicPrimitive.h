#pragma once
#include "PhysGeom.h"

class NewtonBody;

namespace GraphysX
{
	class CLKinematicPrimitive : public CLPhysGeom
	{
	public:
		CLKinematicPrimitive(NewtonWorld* world, STEntityParam param);
		virtual ~CLKinematicPrimitive();

		virtual void update() override;

	protected:
		virtual NewtonBody* loadBody();
		virtual void ForceNTorque();
		virtual void Transform();
	};

}