#include "stdafx.h"
#include "DynamicMesh.h"
#include "TV3DMoteur.h"

namespace GraphysX
{
	int CLDynamicMesh::nbrDynPrim = 0;
//	std::vector<CLDynamicMesh*> CLDynamicMesh::vecDynPrim;

	CLDynamicMesh::CLDynamicMesh(NewtonWorld* world, STMeshParam param, ePHYSMAT physMat, float masse) : CLPhysGeom(world, param, physMat, masse)
	{		
	//	vecDynPrim.push_back(this);
		//NewtonBodySetUserData(pbody, this);
		nbrDynPrim++;
	}	
	CLDynamicMesh::~CLDynamicMesh()
	{
	//	vecDynPrim.erase((std::remove(vecDynPrim.begin(), vecDynPrim.end(), this)), vecDynPrim.end());
		std::cout << "DynPrim destructor : " << stParam.name << std::endl;
		nbrDynPrim--;
	}
	int CLDynamicMesh::getNbrDynPrim()
	{
		return nbrDynPrim;
	}
	void CLDynamicMesh::ForceNTorque()
	{
		addGravity();
	}
	void CLDynamicMesh::Transform()
	{
		
		cTV_3DMATRIX* mat = matrix_bodyToTV();
		//float* val =(float*)mat;
		/*float* val = &mat->m[0][0];
		std::cout << std::endl << mesh->GetMeshName() << " - applyMat:" << std::endl;
		for (int i = 0; i < 16; i++)
		{
		std::cout << *val << " ";
		val++;
		if (i == 3) std::cout << std::endl;
		if (i == 7) std::cout << std::endl;
		if (i == 11) std::cout << std::endl;

		}*/
		mesh->SetMatrix(mat);
	}
}