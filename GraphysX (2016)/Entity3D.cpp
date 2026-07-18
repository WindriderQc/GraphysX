#include "stdafx.h"
#include "Entity3D.h"
#include "Scene.h"

namespace GraphysX
{
	int CLEntity3D::nbrEntity = 0;
	//std::vector<CLEntity3D*> CLEntity3D::vecEntity;

	CLEntity3D::CLEntity3D()
	{
		//pScene->vEntity.push_back(this);
		nbrEntity++;
		entityID = nbrEntity;
	}
	CLEntity3D::~CLEntity3D()
	{
		//pScene->vEntity.erase((std::remove(pScene->vEntity.begin(), pScene->vEntity.end(), this)), pScene->vEntity.end());
		nbrEntity--;
	}
	int CLEntity3D::getNbrEntity()
	{
		return nbrEntity;
	}

}