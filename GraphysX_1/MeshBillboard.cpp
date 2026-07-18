#include "stdafx.h"
#include "MeshBillboard.h"
#include "ResourceManager.h"
#include "TV3DMoteur.h"


namespace GraphysX
{
	CLMeshBillboard::CLMeshBillboard(STEntityParam param, int texture) : CLMeshPrimitive(param), tex(texture)
	{}
	CLMeshBillboard::~CLMeshBillboard()
	{}
	CTVMesh* CLMeshBillboard::loadMesh() 
	{
		mesh = GraphysX::CLTV3D::pTVScene.CreateBillboard(tex, stParam.pos.x, stParam.pos.y, stParam.pos.z, stParam.scale.x, stParam.scale.y, stParam.name.c_str(), false);
		mesh->SetRotation(stParam.rot.x, stParam.rot.y, stParam.rot.z);
		mesh->SetAlphaTest(true);
		setMeshUponParam();
		mesh->SetLightingMode(cCONST_TV_LIGHTINGMODE::cTV_LIGHTING_MANAGED);
		return(mesh);
	}
}