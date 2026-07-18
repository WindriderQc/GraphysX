#pragma once
#include "PhysGeom.h"

namespace GraphysX
{
	class CLDynamicMesh : public CLPhysGeom
	{
	public:
		GRAPHYSX_API CLDynamicMesh(NewtonWorld* world, STMeshParam param, ePHYSMAT physMat, float masse = 0.0f);
		virtual ~CLDynamicMesh();

		static int getNbrDynPrim();
		
		//static std::vector<CLDynamicMesh*> getvecDynPrim() { return vecDynPrim; }

	protected:
		virtual void ForceNTorque();
		virtual void Transform();

	private:
		//static std::vector<CLDynamicMesh*> vecDynPrim;
		static int nbrDynPrim;
	};

}