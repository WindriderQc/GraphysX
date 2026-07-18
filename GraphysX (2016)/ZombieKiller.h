#pragma once
#include "Human.h"
#include "Speedo.h"



class Spline3D;

namespace GraphysX
{
	class ZombieKiller : public Human
	{
	public:
		GRAPHYSX_API ZombieKiller(CLMeshPrimitive* cage, CLMeshPrimitive* insidePrim, NewtonWorld* world, STMeshParam param, float masse = 1.0f);
		GRAPHYSX_API virtual ~ZombieKiller();

		GRAPHYSX_API void update() override;
		GRAPHYSX_API virtual void ForceNTorque() override;
		GRAPHYSX_API virtual void Transform() override;
		GRAPHYSX_API CLMeshPrimitive* getInsideMesh();

		

		cTV_3DVECTOR* getMouseDirection() { return &MouseDirection; }
		void setMouse3Dpos(cTV_3DVECTOR pos) { mouse3Dpos = pos; }
		
		bool bThrottle = false;
		bool bJumping = false;
		bool bAllowJumps = false;
		sLine3D BallDirectionTag;

	private:
		CTVMathLibrary math;
		CLSpeedo clSpeedo;
		CLMeshPrimitive* insideBall;
		CLMeshPrimitive* outsideBall;

		// Info pour échange avec physics engine
		dVector TorqueToApply;
		float fThrottleStep;
		float fThrottle;
		float fMaxThrottle;

		
		cTV_3DVECTOR MouseDirection;
		cTV_3DVECTOR mouse3Dpos;
		
		
	};

}