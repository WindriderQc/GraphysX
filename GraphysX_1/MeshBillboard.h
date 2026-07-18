#pragma once
#include "MeshPrimitive.h"

class CTVMesh;

namespace GraphysX
{
	class CLMeshBillboard :
		public CLMeshPrimitive
	{
	public:
		CLMeshBillboard(STEntityParam param, int texture);
		virtual ~CLMeshBillboard();

		virtual CTVMesh* loadMesh() override;
	protected:
		int tex;
	};

}